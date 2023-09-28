/**
 * @file write_vehicle_proximity_migration_data_node.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta/net_array.hpp"
#include "hooking.hpp"
#include "script_global.hpp"
#include "script_local.hpp"

#include <netsync/nodes/vehicle/CVehicleProximityMigrationDataNode.hpp>

namespace big
{
	void hooks::write_vehicle_proximity_migration_data_node(rage::netObject* veh, CVehicleProximityMigrationDataNode* node)
	{
		CVehicle* vehicle = *(CVehicle**)(((__int64)veh) - 432);

		g_hooking->get_original<hooks::write_vehicle_proximity_migration_data_node>()(veh, node);

		if (auto it = g.m_remote_player_teleports.find(vehicle->m_net_object->m_object_id);
		    it != g.m_remote_player_teleports.end())
		{
			node->m_has_occupants[0]  = true;
			node->m_occupants[0]      = it->second.m_player_net_id;
			node->m_override_position = true;
			node->m_position          = it->second.m_position;
			node->m_velocity.x        = 1;
			node->m_velocity.y        = 1;
			node->m_velocity.z        = 1;
		}
	}
}