/**
 * @file view_player_kicks_and_crashes.cpp
 * @brief Options to kick or crash selected player.
 * @note Define `ENABLE_CRASHES` to enable crashes.
 * @todo Fully remove crashes.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "util/ped.hpp"
#include "util/teleport.hpp"
#include "util/toxic.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_kicks_and_crashes()
	{
		std::string title = std::format("Player Kick Options: {}", g_player_service->get_selected()->get_name());
		ImGui::Text(title.c_str());
		auto const is_session_host = [] {
			return gta_util::get_network()->m_game_session_ptr->is_host();
		};

		if (!g_player_service->get_self()->is_host())
				ImGui::Text("Host and breakup kick require session host");

		ImGui::BeginDisabled(!g_player_service->get_self()->is_host());

		components::player_command_button<"hostkick">(g_player_service->get_selected());
		components::player_command_button<"breakup">(g_player_service->get_selected());
		ImGui::SameLine();
		components::command_checkbox<"breakupcheating">();

		ImGui::EndDisabled();

		components::player_command_button<"multikick">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"nfkick">(g_player_service->get_selected());

		components::player_command_button<"oomkick">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"shkick">(g_player_service->get_selected());

		components::player_command_button<"endkick">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"desync">(g_player_service->get_selected());

#ifdef ENABLE_CRASHES
		ImGui::SeparatorText("Crashes (shit)");
		components::player_command_button<"slodpedcrash">(g_player_service->get_selected());
		components::player_command_button<"tsecrash">(g_player_service->get_selected());
#endif // ENABLE_CRASHES
	}
}