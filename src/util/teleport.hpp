/**
 * @file teleport.hpp
 * @brief Teleportation related functions.
 * 
 * @copyright GNU General Public License Version 2.
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
		Entity ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());

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

			g.m_tp_position      = {coords.x, coords.y, coords.z};
			g.m_tp_player_net_id = player->get_ped()->m_net_object->m_object_id;
			g.m_tp_veh_net_id    = g_pointers->m_handle_to_ptr(hnd)->m_net_object->m_object_id;

			if ((player->is_valid() && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id()), false))
			    || PLAYER::IS_REMOTE_PLAYER_IN_NON_CLONED_VEHICLE(player->id()))
				g_pointers->m_clear_ped_tasks_network(player->get_ped(), true);

			for (int i = 0; i < 15; i++)
			{
				script::get_current()->yield(50ms);

				if (auto ptr = (rage::CDynamicEntity*)g_pointers->m_handle_to_ptr(hnd))
				{
					if (auto netobj = ptr->m_net_object)
					{
						g_pointers->m_migrate_object(player->get_net_game_player(), netobj, 3);
					}
				}
			}

			entity::delete_entity(hnd);

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
}
