/**
 * @file view_player_vehicle.cpp
 * @brief Vehicle related options for selected player.
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
	void view::view_player_vehicle()
	{
		components::player_command_button<"vehkick">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"flyingveh">(g_player_service->get_selected(), {});

		components::player_command_button<"boostveh">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"stopveh">(g_player_service->get_selected(), {});

		components::player_command_button<"flip180">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"rcplayer">(g_player_service->get_selected(), {});

		components::player_command_button<"killengine">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"burstwheels">(g_player_service->get_selected(), {});

		components::player_command_button<"smashwindows">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"blacktint">(g_player_service->get_selected(), {});

		components::player_command_button<"lockveh">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"unlockveh">(g_player_service->get_selected(), {});

		components::player_command_button<"opendoors">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"closedoors">(g_player_service->get_selected(), {});

		components::player_command_button<"breakdoors">(g_player_service->get_selected(), {});

		components::player_command_button<"upgradeveh">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"downgradeveh">(g_player_service->get_selected(), {});

		components::player_command_button<"svehjump">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"svehboost">(g_player_service->get_selected(), {});

		components::player_command_button<"sshuntleft">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"sshuntright">(g_player_service->get_selected(), {});
	}
}