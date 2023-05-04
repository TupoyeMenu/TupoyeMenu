/**
 * @file matchmaking_service.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "matchmaking_service.hpp"

#include "hooking.hpp"
#include "pointers.hpp"
#include "script.hpp"

#include <network/Network.hpp>

namespace big
{
	matchmaking_service::matchmaking_service()
	{
		g_matchmaking_service = this;
	}

	matchmaking_service::~matchmaking_service()
	{
		g_matchmaking_service = nullptr;
	}

	bool matchmaking_service::matchmake(std::optional<int> constraint)
	{
		for (auto& session : m_found_sessions)
		{
			session.is_valid = true;
		}

		NetworkGameFilterMatchmakingComponent component{};
		strcpy(component.m_filter_name, "Group");
		component.m_game_mode      = 0;
		component.m_num_parameters = 0;

		if (g.session_browser.region_filter_enabled)
		{
			component.SetParameter("MMATTR_REGION", 0, g.session_browser.region_filter);
		}

		if (constraint)
		{
			component.SetParameter("MMATTR_DISCRIMINATOR", 1, constraint.value());
		}

		rage::rlTaskStatus state{};
		static rage::rlSessionInfo result_sessions[MAX_SESSIONS_TO_FIND];

		m_active = true;

		if (g_hooking->get_original<hooks::start_matchmaking_find_sessions>()(0, 1, &component, MAX_SESSIONS_TO_FIND, result_sessions, &m_num_sessions_found, &state))
		{
			while (state.status == 1)
				script::get_current()->yield();

			if (state.status == 3)
			{
				for (int i = 0; i < m_num_sessions_found; i++)
				{
					m_found_sessions[i].info = result_sessions[i];

					if (constraint && m_found_sessions[i].attributes.player_count >= 30)
						m_found_sessions[i].is_valid = false;

					if (g.session_browser.language_filter_enabled
					    && m_found_sessions[i].attributes.language != g.session_browser.language_filter)
						m_found_sessions[i].is_valid = false;

					if (g.session_browser.player_count_filter_enabled
					    && (m_found_sessions[i].attributes.player_count < g.session_browser.player_count_filter_minimum
					        || m_found_sessions[i].attributes.player_count > g.session_browser.player_count_filter_maximum))
					{
						m_found_sessions[i].is_valid = false;
					}

					if (g.session_browser.pool_filter_enabled
					    && ((m_found_sessions[i].attributes.discriminator & (1 << 14)) == (1 << 14))
					        != (bool)g.session_browser.pool_filter)
						m_found_sessions[i].is_valid = false;
				}

				if (g.session_browser.sort_method != 0)
				{
					std::qsort(m_found_sessions, m_num_sessions_found, sizeof(session), [](const void* a1, const void* a2) -> int {
						std::strong_ordering result;

						if (g.session_browser.sort_method == 1)
						{
							result = (((session*)(a1))->attributes.player_count <=> ((session*)(a2))->attributes.player_count);
						}

						if (result == 0)
							return 0;

						if (result > 0)
							return g.session_browser.sort_direction ? -1 : 1;

						if (result < 0)
							return g.session_browser.sort_direction ? 1 : -1;


						std::unreachable();
					});
				}

				m_active = false;
				return true;
			}
		}
		else
		{
			m_active = false;
			return false;
		}

		m_active = false;
		return false;
	}
}