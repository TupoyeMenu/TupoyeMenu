/**
 * @file serialize_vehicle_gadget_data_node.cpp
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

#include <netsync/nodes/vehicle/CVehicleGadgetDataNode.hpp>

namespace big
{
	void hooks::serialize_vehicle_gadget_data_node(CVehicleGadgetDataNode* node, rage::CSyncDataBase* data)
	{
		data->SerializeBool(&node->m_has_parent_offset);
		if (node->m_has_parent_offset || data->IsSizeCalculator())
		{
			data->SerializeSignedFloat((float*)&node->m_parent_offset_x, 24.0f, 14);
			data->SerializeSignedFloat((float*)&node->m_parent_offset_y, 24.0f, 14);
			data->SerializeSignedFloat((float*)&node->m_parent_offset_z, 24.0f, 14);
		}
		else
		{
			node->m_parent_offset_x = 0;
			node->m_parent_offset_y = 0;
			node->m_parent_offset_z = 0;
			node->m_parent_offset_w = 0;
		}

		data->SerializeDwordAlt(&node->m_gadget_count, 2);

		if (data->IsSizeCalculator())
			node->m_gadget_count = 2;

		if (node->m_gadget_count > 2)
		{
			notify::crash_blocked(g.m_syncing_player, "out of bounds vehicle gadget count");
			return;
		}

		for (uint32_t i = 0; i < node->m_gadget_count; i++)
		{
			data->SerializeDwordAlt((uint32_t*)&node->m_gadget_data[i].m_gadget_type, 3);

			int size;
			if (data->IsSizeCalculator())
				size = 94;
			else
				size = g_pointers->m_gta.m_get_vehicle_gadget_array_size(node->m_gadget_data[i].m_gadget_type);

			data->SerializeArray(&node->m_gadget_data[i].m_data, size);
		}
	}
}