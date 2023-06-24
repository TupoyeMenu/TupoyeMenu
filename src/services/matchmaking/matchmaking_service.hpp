/**
 * @file matchmaking_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big
{
	class matchmaking_service
	{
	public:
		constexpr static int MAX_SESSIONS_TO_FIND = 1000;

		struct session_attributes
		{
			int discriminator;
			int player_count;
			int region;
			int language;
		};

		struct session
		{
			rage::rlSessionInfo info;
			session_attributes attributes;
			bool is_valid;
		};

	private:
		int m_num_sessions_found = 0;
		bool m_active            = false;
		session m_found_sessions[MAX_SESSIONS_TO_FIND];

	public:
		matchmaking_service();
		~matchmaking_service();
		bool matchmake(std::optional<int> constraint = std::nullopt);

		inline int get_num_found_sessions()
		{
			return m_num_sessions_found;
		}

		inline session* get_found_sessions()
		{
			return m_found_sessions;
		}

		inline bool is_active()
		{
			return m_active;
		}
	};

	inline matchmaking_service* g_matchmaking_service;
}