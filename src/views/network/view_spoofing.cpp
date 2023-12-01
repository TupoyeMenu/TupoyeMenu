/**
 * @file view_spoofing.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/blip_types.hpp"
#include "core/data/language_codes.hpp"
#include "core/data/pool_types.hpp"
#include "core/data/region_codes.hpp"
#include "fiber_pool.hpp"
#include "imgui.h"
#include "util/teleport.hpp"
#include "views/view.hpp"

#include <network/ClanData.hpp>

namespace big
{
	void view::spoofing()
	{
		ImGui::TextUnformatted("Detection Avoidance");
		ImGui::Checkbox("Hide God Mode", &g.spoofing.spoof_hide_god);
		ImGui::Checkbox("Hide Spectate", &g.spoofing.spoof_hide_spectate);

		ImGui::SeparatorText("Player List Spoof");

		ImGui::Checkbox("Hide From Player List", &g.spoofing.hide_from_player_list);

		ImGui::Checkbox("Spoof Rank", &g.spoofing.spoof_rank);
		if (g.spoofing.spoof_rank)
		{
			ImGui::SameLine();
			if (ImGui::InputInt("###rank", &g.spoofing.rank))
			{
				*g_pointers->m_gta.m_force_player_card_refresh = true;
			}
		}

		ImGui::Checkbox("Spoof K/D Ratio", &g.spoofing.spoof_kd_ratio);
		if (g.spoofing.spoof_kd_ratio)
		{
			ImGui::SameLine();
			if (ImGui::InputFloat("###kd_ratio", &g.spoofing.kd_ratio))
			{
				*g_pointers->m_gta.m_force_player_card_refresh = true;
			}
		}

		ImGui::Checkbox("Spoof Badsport State", &g.spoofing.spoof_bad_sport);
		if (g.spoofing.spoof_bad_sport)
		{
			ImGui::SameLine();
			if (ImGui::Combo("###badsport_select", &g.spoofing.badsport_type, "Clean Player\0Dirty Player\0Bad Sport"))
			{
				*g_pointers->m_gta.m_force_player_card_refresh = true;
			}
		}

		ImGui::Checkbox("Spoof Job Points", &g.spoofing.spoof_job_points);
		if (g.spoofing.spoof_job_points)
		{
			ImGui::SameLine();
			ImGui::InputInt("###jp", &g.spoofing.job_points);
		}

		ImGui::SeparatorText("Misc");

		components::script_patch_checkbox("Spoof Blip Type", &g.spoofing.spoof_blip);
		if (g.spoofing.spoof_blip)
		{
			ImGui::SameLine();
			if (ImGui::BeginCombo("###blip_type_select", blip_types[g.spoofing.blip_type]))
			{
				for (int i = 0; i < blip_types.size(); i++)
				{
					if (ImGui::Selectable(blip_types[i], g.spoofing.blip_type == i))
					{
						g.spoofing.blip_type = i;
					}
				}
				ImGui::EndCombo();
			}
		}

		ImGui::Checkbox("Spoof Player Model", &g.spoofing.spoof_player_model);
		if (g.spoofing.spoof_player_model)
		{
			static char model[32];
			strcpy_s(model, sizeof(model), g.spoofing.player_model.c_str());

			ImGui::SameLine();
			components::input_text("##model_input", model, sizeof(model));

			if (model != g.spoofing.player_model)
				g.spoofing.player_model = std::string(model);
		}

		ImGui::SeparatorText("Session Pool Spoof");
		if (ImGui::BeginCombo("Pools", pool_types[g.spoofing.pool_type]))
		{
			for (int i = 0; i < pool_types.size(); i++)
			{
				if (ImGui::Selectable(pool_types[i], g.spoofing.pool_type == i))
				{
					g.spoofing.pool_type = i;
				};
			}
			ImGui::EndCombo();
		}

		ImGui::SeparatorText("Crew Spoof");

		ImGui::Checkbox("Spoof Crew", &g.spoofing.spoof_crew_data);

		constexpr size_t crew_tag_size = RTL_FIELD_SIZE(ClanData, m_clan_tag);
		static char crew_tag[crew_tag_size];
		strcpy_s(crew_tag, sizeof(crew_tag), g.spoofing.crew_tag.c_str());

		ImGui::TextUnformatted("Crew Tag:");
		components::input_text("##crew_tag_input", crew_tag, sizeof(crew_tag));

		if (crew_tag != g.spoofing.crew_tag)
			g.spoofing.crew_tag = std::string(crew_tag);

		ImGui::Checkbox("Is Rockstar Crew", &g.spoofing.rockstar_crew);

		ImGui::Checkbox("Square Crew Tag", &g.spoofing.square_crew_tag);

		ImGui::SeparatorText("Session Attribute Spoof");
		components::small_text("Only works when session host");

		ImGui::Checkbox("Region", &g.spoofing.spoof_session_region_type);
		if (g.spoofing.spoof_session_region_type)
		{
			ImGui::SameLine();
			if (ImGui::BeginCombo("###region_select", regions[g.spoofing.session_region_type].name))
			{
				for (const auto& region : regions)
				{
					if (ImGui::Selectable(region.name, g.spoofing.session_region_type == region.id))
					{
						g.spoofing.session_region_type = region.id;
					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::Checkbox("Language", &g.spoofing.spoof_session_language);
		if (g.spoofing.spoof_session_language)
		{
			ImGui::SameLine();

			if (ImGui::BeginCombo("###language_select", languages[g.spoofing.session_language].name))
			{
				for (const auto& language : languages)
				{
					if (ImGui::Selectable(language.name, g.spoofing.session_language == language.id))
					{
						g.spoofing.session_language = language.id;
					};
				}
				ImGui::EndCombo();
			}
		}

		ImGui::Checkbox("Player Count", &g.spoofing.spoof_session_player_count);
		if (g.spoofing.spoof_session_player_count)
		{
			ImGui::SameLine();
			ImGui::InputInt("###player_count", &g.spoofing.session_player_count);
		}

		ImGui::Checkbox("Spoof Session Bad Sport Status", &g.spoofing.spoof_session_bad_sport_status);
		if (g.spoofing.spoof_session_bad_sport_status)
		{
			ImGui::SameLine();
			ImGui::Checkbox("Badsport", &g.spoofing.session_bad_sport);
		}
	}
}
