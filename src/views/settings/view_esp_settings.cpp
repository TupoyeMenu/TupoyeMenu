/**
 * @file view_esp_settings.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "views/view.hpp"

namespace big
{
	void view::esp_settings()
	{
		ImGui::Checkbox("ESP Enabled", &g.esp.enabled);

		ImGui::BeginDisabled(!g.esp.enabled);
		ImGui::TextUnformatted("Global Render Distance (min, max)");
		ImGui::SliderFloat2("###Global Render Distance", g.esp.global_render_distance, 0.f, 1500.f);

		ImGui::Checkbox("Tracer", &g.esp.tracer);
		if (g.esp.tracer)
		{
			ImGui::TextUnformatted("Tracer Draw Position (x, y)");
			ImGui::SliderFloat2("###Draw Position", g.esp.tracer_draw_position, 0.f, 1.f);
			ImGui::TextUnformatted("Tracer Render Distance (min, max)");
			ImGui::SliderFloat2("###Tracer Render Distance",
			    g.esp.tracer_render_distance,
			    g.esp.global_render_distance[0],
			    g.esp.global_render_distance[1]);
		}

		ImGui::Checkbox("Box ESP", &g.esp.box);
		if (g.esp.box)
		{
			ImGui::TextUnformatted("Box Render Distance (min, max)");
			ImGui::SliderFloat2("###Box Render Distance",
			    g.esp.box_render_distance,
			    g.esp.global_render_distance[0],
			    g.esp.global_render_distance[1]);
		}

		ImGui::Checkbox("Show Player Name", &g.esp.name);
		ImGui::Checkbox("Show Player Distance", &g.esp.distance);
		ImGui::Checkbox("Show Player Godmode", &g.esp.god);
		ImGui::Checkbox("Show Player Health", &g.esp.health);
		ImGui::Checkbox("Show Player Armor", &g.esp.armor);

		ImGui::Checkbox("Should ESP Color Change with Distance", &g.esp.change_esp_color_from_dist);
		if (g.esp.health)
			ImGui::Checkbox("Should Health bar Scale with Distance", &g.esp.scale_health_from_dist);

		if (g.esp.armor)
			ImGui::Checkbox("Should Armor bar Scale with Distance", &g.esp.scale_armor_from_dist);

		static ImVec4 col_enemy      = ImGui::ColorConvertU32ToFloat4(g.esp.enemy_color);
		static ImVec4 col_enemy_near = ImGui::ColorConvertU32ToFloat4(g.esp.enemy_near_color);
		static ImVec4 col_default    = ImGui::ColorConvertU32ToFloat4(g.esp.default_color);
		static ImVec4 col_friend     = ImGui::ColorConvertU32ToFloat4(g.esp.friend_color);

		ImGui::TextUnformatted("Distance threshold (min, max)");
		ImGui::SliderFloat2("###Distance threshold", g.esp.distance_threshold, g.esp.global_render_distance[0], g.esp.global_render_distance[1]);

		if (ImGui::TreeNode("ESP Colors (RGBA)"))
		{
			if (g.esp.change_esp_color_from_dist)
			{
				ImGui::TextUnformatted("Enemy Close Color:");
				if (ImGui::ColorEdit4("###Enemy ESP Color##esp_picker", (float*)&col_enemy, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
				{
					g.esp.enemy_color = ImGui::ColorConvertFloat4ToU32(col_enemy);
				}

				ImGui::TextUnformatted("Enemy Near Color:");
				if (ImGui::ColorEdit4("###Enemy Near ESP Color##esp_picker", (float*)&col_enemy_near, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
				{
					g.esp.enemy_near_color = ImGui::ColorConvertFloat4ToU32(col_enemy_near);
				}
			}

			ImGui::TextUnformatted("Default Color:");
			if (ImGui::ColorEdit4("###Default ESP Color##esp_picker", (float*)&col_default, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.esp.default_color = ImGui::ColorConvertFloat4ToU32(col_default);
			}

			ImGui::TextUnformatted("Friendly Color:");
			if (ImGui::ColorEdit4("###Friend ESP Color##friend_picker", (float*)&col_friend, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.esp.friend_color = ImGui::ColorConvertFloat4ToU32(col_friend);
			}
		}

		ImGui::EndDisabled();
	}
}
