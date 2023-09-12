/**
 * @file entity.hpp
 * @brief Basic entity related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "gta/joaat.hpp"
#include "gta_util.hpp"
#include "math.hpp"
#include "natives.hpp"
#include "pools.hpp"
#include "script.hpp"
#include "services/players/player_service.hpp"

namespace big::entity
{
	void cage_ped(Ped ped);
	void clean_ped(Ped ped);
	bool take_control_of(Entity ent, int timeout = 300);
	void delete_entity(Entity& ent, bool force = false);
	bool raycast(Entity* ent);
	bool raycast(Vector3* endcoor);
	bool network_has_control_of_entity(rage::netObject* net_object);
	std::vector<Entity> get_entities(bool vehicles, bool peds, bool props = false, bool include_self_veh = false);
	bool load_ground_at_3dcoord(Vector3& location);
	double distance_to_middle_of_screen(const rage::fvector2& screen_pos);
	Entity get_entity_closest_to_middle_of_screen(rage::fwEntity** pointer = nullptr, std::vector<Entity> ignore_entities = {}, bool include_veh = true, bool include_ped = true, bool include_prop = true, bool include_players = true);
}
