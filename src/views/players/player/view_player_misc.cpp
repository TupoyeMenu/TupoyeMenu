/**
 * @file view_player_misc.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "util/scripts.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_misc()
	{
		std::string title = std::format("Player Misc Options: {}", g_player_service->get_selected()->get_name());
		ImGui::Text(title.c_str());

		components::player_command_button<"joinceo">(g_player_service->get_selected());
		components::player_command_button<"enterint">(g_player_service->get_selected());
		components::player_command_button<"copyoutfit">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"copymodel">(g_player_service->get_selected());

		components::player_command_button<"clearwanted">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"givehealth">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"givearmor">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"giveammo">(g_player_service->get_selected());

		ImGui::Checkbox("Off The Radar", &g_player_service->get_selected()->off_radar);
		ImGui::Checkbox("Never Wanted", &g_player_service->get_selected()->never_wanted);
		ImGui::Checkbox("Semi Godmode", &g_player_service->get_selected()->semi_godmode);

		components::button("Gooch Test", [] {
			*script_global(1890378).at(289).at(1).as<Player*>() = g_player_service->get_selected()->id();
			scripts::start_launcher_script(171);
		});
	}
}