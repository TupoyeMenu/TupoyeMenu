/**
 * @file view_player_troll.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "util/teleport.hpp"
#include "util/troll.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_troll()
	{
		std::string title = std::format("Player Troll Options: {}", g_player_service->get_selected()->get_name());
		ImGui::TextUnformatted(title.c_str());

		components::player_command_button<"playertp">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"bring">(g_player_service->get_selected());

		components::player_command_button<"playervehtp">(g_player_service->get_selected());
		components::player_command_button<"rcplayer">(g_player_service->get_selected());

		components::button("Godmode kill", [] {
			teleport::godmode_kill(g_player_service->get_selected());
		});
		ImGui::SameLine();
		components::help_marker("Teleports players vehicle into killbox. \nBlocked by most internal menus.");

		static int bounty_value = 0;

		ImGui::SliderInt("Bounty", &bounty_value, 0, 10000);
		ImGui::SameLine();
		components::command_checkbox<"anonbounty">();
		ImGui::SameLine();
		components::button("Set", [] {
			troll::set_bounty_on_player(g_player_service->get_selected(), bounty_value, g.session.anonymous_bounty);
		});
	}
}