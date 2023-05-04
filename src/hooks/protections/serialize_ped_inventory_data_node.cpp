/**
 * @file serialize_ped_inventory_data_node.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta/net_game_event.hpp"
#include "hooking.hpp"
#include "util/notify.hpp"

#include <netsync/nodes/ped/CPedInventoryDataNode.hpp>

namespace big
{
	void hooks::serialize_ped_inventory_data_node(CPedInventoryDataNode* node, rage::CSyncDataBase* data)
	{
		if (data->IsSizeCalculator())
		{
			node->m_num_items = 105;
			node->m_num_ammos = 65;
		}

		data->SerializeDwordAlt(&node->m_num_items, 7);
		if (node->m_num_items > 105)
		{
			notify::crash_blocked(g.m_syncing_player, "out of bounds inventory item count");
			return;
		}

		for (uint32_t i = 0; i < node->m_num_items; i++)
		{
			data->SerializeDwordAlt(&node->m_items[i], 9);
			node->unk_1680[i] = 0;
			node->unk_16E9[i] = 0;
		}

		data->SerializeDwordAlt(&node->m_num_ammos, 7);
		data->SerializeBool(&node->m_ammo_all_infinite);
		if (node->m_num_ammos > 65)
		{
			notify::crash_blocked(g.m_syncing_player, "out of bounds inventory ammo count");
			return;
		}

		for (uint32_t i = 0; i < node->m_num_ammos; i++)
		{
			data->SerializeDwordAlt(&node->m_ammos[i], 9);
			if (node->m_ammo_all_infinite && !data->IsSizeCalculator())
				continue;
			data->SerializeBool(&node->m_infinite_ammos[i]);
			if (node->m_infinite_ammos[i] && !data->IsSizeCalculator())
				continue;
			data->SerializeDwordAlt(&node->m_ammo_quantities[i], 14);
		}
	}
}