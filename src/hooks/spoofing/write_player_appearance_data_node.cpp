/**
 * @file write_player_appearance_data_node.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "util/model_info.hpp"

#include <netsync/nodes/player/CPlayerAppearanceDataNode.hpp>

namespace big
{
	void hooks::write_player_appearance_data_node(rage::netObject* player, CPlayerAppearanceDataNode* node)
	{
		g_hooking->get_original<hooks::write_player_appearance_data_node>()(player, node);

		if (g.spoofing.spoof_player_model)
		{
			if (const auto model_hash = rage::joaat(g.spoofing.player_model); model_info::does_model_exist(model_hash))
			{
				node->m_model_hash = model_hash;
			}
			node->m_has_head_blend_data         = false;
			node->components.m_component_bitset = 0;
		}
	}
}
