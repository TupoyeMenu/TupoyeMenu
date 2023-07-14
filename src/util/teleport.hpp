/**
 * @file teleport.hpp
 * @brief Teleportation related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "blip.hpp"
#include "entity.hpp"
#include "gta/enums.hpp"
#include "gta/net_object_mgr.hpp"
#include "services/players/player_service.hpp"
#include "vehicle.hpp"

namespace big::teleport
{
	inline bool teleport_player_to_coords(player_ptr player, Vector3 coords)
	{
		Entity ent;

		if (*g_pointers->m_gta.m_is_session_started)
			ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
		else
			ent = self::ped;

		if (ent == self::ped || ent == self::veh)
			PED::SET_PED_COORDS_KEEP_VEHICLE(ent, coords.x, coords.y, coords.z);

		if (ENTITY::IS_ENTITY_DEAD(ent, true))
		{
			g_notification_service->push_warning("Teleport", "Target player is dead.");
			return false;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(ent, true))
		{
			ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

			if (entity::take_control_of(ent))
				ENTITY::SET_ENTITY_COORDS(ent, coords.x, coords.y, coords.z, 0, 0, 0, 0);
			else
				g_notification_service->push_warning("Teleport", "Failed to take control of player vehicle.");

			return true;
		}
		else
		{
			auto hnd = vehicle::spawn(RAGE_JOAAT("ninef"), *player->get_ped()->get_position(), 0.0f, true);
			ENTITY::SET_ENTITY_VISIBLE(hnd, false, false);
			ENTITY::SET_ENTITY_COLLISION(hnd, false, false);
			ENTITY::FREEZE_ENTITY_POSITION(hnd, true);

			auto obj_id                      = player->get_ped()->m_net_object->m_object_id;
			remote_player_teleport remote_tp = {obj_id, {coords.x, coords.y, coords.z}};

			g.m_remote_player_teleports.emplace(g_pointers->m_gta.m_handle_to_ptr(hnd)->m_net_object->m_object_id, remote_tp);

			if ((player->is_valid() && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id()), false))
			    || PLAYER::IS_REMOTE_PLAYER_IN_NON_CLONED_VEHICLE(player->id()))
				g_pointers->m_gta.m_clear_ped_tasks_network(player->get_ped(), true);

			for (int i = 0; i < 15; i++)
			{
				script::get_current()->yield(50ms);

				if (auto ptr = (rage::CDynamicEntity*)g_pointers->m_gta.m_handle_to_ptr(hnd))
				{
					if (auto netobj = ptr->m_net_object)
					{
						g_pointers->m_gta.m_migrate_object(player->get_net_game_player(), netobj, 3);
					}
				}
			}

			entity::delete_entity(hnd);

			std::erase_if(g.m_remote_player_teleports, [obj_id](auto& obj) {
				return obj.first == obj_id;
			});

			return true;
		}
	}

	inline bool godmode_kill(player_ptr player)
	{
		return teleport_player_to_coords(player, Vector3(883.5, -986.5, 31.3));
	}

	inline bool bring_player(player_ptr player)
	{
		return teleport_player_to_coords(player, self::pos);
	}

	/**
	 * @brief Teleports local ped into vehicle.
	 * 
	 * @param veh Vehicle to teleport into.
	 * @return True if teleport is successful.
	 */
	inline bool into_vehicle(Vehicle veh)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh))
		{
			g_notification_service->push_warning("Teleport", "No valid vehicle could be found.");

			return false;
		}

		int seat_index = 255;
		if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1, true))
			seat_index = -1;
		else if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -2, true))
			seat_index = -2;

		if (seat_index == 255)
		{
			g_notification_service->push_warning("Teleport", "No seats are free in the player vehicle.");

			return false;
		}

		Vector3 location = ENTITY::GET_ENTITY_COORDS(veh, true);
		entity::load_ground_at_3dcoord(location);

		Ped ped = self::ped;

		ENTITY::SET_ENTITY_COORDS(ped, location.x, location.y, location.z, 0, 0, 0, 0);

		script::get_current()->yield();

		PED::SET_PED_INTO_VEHICLE(ped, veh, seat_index);

		return true;
	}

	/**
	 * @brief Teleports local ped to coords.
	 * 
	 * @param location Coords to teleport to.
	 */
	inline void to_coords(Vector3 location)
	{
		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z + 1.f);
	}

	/**
	 * @brief Teleports local ped to blip.
	 * 
	 * @param sprite Blip sprite to search for.
	 * @param color Blip color to search for, set to -1 to ignore.
	 * @return True if teleport is successful.
	 */
	inline bool to_blip(int sprite, int color = -1)
	{
		Vector3 location;

		if (!blip::get_blip_location(location, sprite, color))
			return false;

		if (sprite == (int)BlipIcons::Waypoint)
			entity::load_ground_at_3dcoord(location);

		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z);

		return true;
	}

	/**
	 * @brief Teleports local ped to entity.
	 * 
	 * @param ent Entity to teleport to.
	 */
	inline void to_entity(Entity ent)
	{
		Vector3 location = ENTITY::GET_ENTITY_COORDS(ent, true);

		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z);
	}

	/**
	 * @brief Teleports local ped to player.
	 * 
	 * @param player Player to teleport to.
	 */
	inline void to_player(Player player)
	{
		to_entity(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
	}

	/**
	 * @brief Teleport local ped to map waypoint.
	 * 
	 * @return True if teleport is successful.
	 */
	inline bool to_waypoint()
	{
		if (!to_blip((int)BlipIcons::Waypoint))
		{
			g_notification_service->push_warning("Teleport", "Failed to find waypoint position.");

			return false;
		}
		return true;
	}

	/**
	 * @brief Teleport local ped to objective blip.
	 * 
	 * @return True if teleport is successful. 
	 */
	inline bool to_objective()
	{
		Vector3 location;

		if (!blip::get_objective_location(location))
		{
			g_notification_service->push_warning("Teleport", "Failed to find objective position");
			return false;
		}

		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z);

		return true;
	}

	inline bool tp_on_top(Entity ent, bool match_velocity)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(ent))
			return false;

		Vector3 ent_dimensions_max, ent_dimensions_min, ent_pos;

		MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(ent), &ent_dimensions_min, &ent_dimensions_max);
		ent_pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, 0, 0, ent_dimensions_max.z);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(self::ped, ent_pos.x, ent_pos.y, ent_pos.z, 0, 0, 0);

		if (match_velocity)
		{
			auto ent_velocity = ENTITY::GET_ENTITY_VELOCITY(ent);
			ENTITY::SET_ENTITY_VELOCITY(self::ped, ent_velocity.x, ent_velocity.y, ent_velocity.z);
		}

		return false;
	}
}
