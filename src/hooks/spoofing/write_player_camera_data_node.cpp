/**
 * @file write_player_camera_data_node.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "services/players/player_service.hpp"
#include "util/globals.hpp"

#include <netsync/nodes/player/CPlayerCameraDataNode.hpp>
#include <netsync/nodes/player/CPlayerGameStateDataNode.hpp>

namespace big
{
	bool hooks::write_player_camera_data_node(rage::netObject* player, CPlayerCameraDataNode* node)
	{
		auto ret = g_hooking->get_original<hooks::write_player_camera_data_node>()(player, node);

		if (g.spoofing.spoof_hide_spectate && g.player.spectating)
		{
			node->m_free_cam_pos_x += 50.0f;
			node->m_free_cam_pos_y -= 50.0f;
			node->m_camera_x -= 50.0f;
		}

		return ret;
	}
}
