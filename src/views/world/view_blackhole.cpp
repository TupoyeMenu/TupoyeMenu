/**
 * @file view_blackhole.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "imgui.h"
#include "util/local_player.hpp"
#include "views/view.hpp"

namespace big
{
	void view::blackhole()
	{
		ImGui::Text("Blackhole");
		
		components::command_checkbox<"blackhole">();

		ImGui::SeparatorText("Entities");
		components::command_checkbox<"blackholeincvehs">();
		ImGui::SameLine();
		components::command_checkbox<"blackholeincpeds">();

		ImGui::SeparatorText("Position");
		ImGui::InputFloat("X", &g.world.blackhole.pos.x, 5.f, 200.f);
		ImGui::InputFloat("Y", &g.world.blackhole.pos.y, 5.f, 200.f);
		ImGui::InputFloat("Z", &g.world.blackhole.pos.z, 5.f, 200.f);
		ImGui::SliderFloat("Scale", &g.world.blackhole.scale, 2.f, 12.f, "%.0f");

		components::button("Set to current coords", [] {
			const auto player_pos = g_local_player->get_position();

			g.world.blackhole.pos.x = player_pos->x;
			g.world.blackhole.pos.y = player_pos->y;
			g.world.blackhole.pos.z = player_pos->z;
		});

		ImGui::SeparatorText("Hole Color");
		ImGui::SetNextItemWidth(214);
		ImGui::ColorPicker3("Color", g.world.blackhole.color, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
		ImGui::SliderInt("Alpha", &g.world.blackhole.alpha, 0, 255);
	}
}