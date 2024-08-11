/**
 * @file teleport.hpp
 * @brief Teleportation related functions.
 */

#pragma once
#include "blip.hpp"
#include "entity.hpp"
#include "fiber_pool.hpp"
#include "gta/enums.hpp"
#include "services/players/player_service.hpp"
#include "vehicle.hpp"
#include "gta/joaat.hpp"

namespace big::teleport
{
	/**
	 * @brief Teleports local ped to coords.
	 * 
	 * @param location Coords to teleport to.
	 */
	inline void to_coords(const Vector3& location, const Vector3& euler = {0, 0, 0})
	{
		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z + 1.f);
		if (euler.x != 0.f)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(self::ped, true))
			{
				ENTITY::SET_ENTITY_HEADING(self::veh, euler.x);
			}
			else
			{
				ENTITY::SET_ENTITY_HEADING(self::ped, euler.x);
			}
		}
		if (euler.y != 0.f && euler.z != 0.f)
		{
			CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(euler.y, 1.f);
			CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(euler.z);
		}
	}

	inline bool teleport_player_to_coords(player_ptr player, Vector3 coords, Vector3 euler = {0, 0, 0})
	{
		Entity ent;

		if (*g_pointers->m_gta.m_is_session_started)
			ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
		else
			ent = self::ped;

		bool is_local_player = ent == self::ped;

		if (is_local_player)
		{
			to_coords(coords, euler);
			return true;
		}

		if (ENTITY::IS_ENTITY_DEAD(ent, true))
		{
			g_notification_service.push_warning("Teleport", "Player is dead, you can't teleport them.");
			return false;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(ent, true))
		{
			ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

			if (entity::take_control_of(ent))
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, coords.x, coords.y, coords.z, TRUE, TRUE, TRUE);
				if (euler.x != 0.0f)
				{
					ENTITY::SET_ENTITY_HEADING(ent, euler.x);
				}
			}
			else
			{
				g_notification_service.push_warning("Teleport", "Failed to take control of entity.");
			}

			return true;
		}
		else
		{
			Vehicle hnd = vehicle::spawn("rcbandito"_J, *player->get_ped()->get_position(), 0.0f, true);

			if (!hnd)
				return false;

			if (!g_pointers->m_gta.m_handle_to_ptr(hnd)->m_net_object)
				return false;

			ENTITY::SET_ENTITY_VISIBLE(hnd, false, false);
			ENTITY::SET_ENTITY_COLLISION(hnd, false, false);
			ENTITY::FREEZE_ENTITY_POSITION(hnd, true);

			auto obj_id                      = player->get_ped()->m_net_object->m_object_id;
			auto veh_id                      = g_pointers->m_gta.m_handle_to_ptr(hnd)->m_net_object->m_object_id;
			remote_player_teleport remote_tp = {obj_id, {coords.x, coords.y, coords.z}};

			g.m_remote_player_teleports.emplace(veh_id, remote_tp);

			if ((player->is_valid() && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id()), false))
			    || PLAYER::IS_REMOTE_PLAYER_IN_NON_CLONED_VEHICLE(player->id()))
				g_pointers->m_gta.m_clear_ped_tasks_network(player->get_ped(), true); // This should never trigger.

			for (int i = 0; i < 30; i++)
			{
				script::get_current()->yield(25ms);

				auto ptr = (rage::CDynamicEntity*)g_pointers->m_gta.m_handle_to_ptr(hnd);

				if (!ptr || !ptr->m_net_object)
					break;

				auto plyr = player->get_net_game_player();

				if (!plyr)
					break;

				g_pointers->m_gta.m_migrate_object(plyr, ptr->m_net_object, 3);

				auto new_coords = ENTITY::GET_ENTITY_COORDS(hnd, true);
				if (SYSTEM::VDIST2(coords.x, coords.y, coords.z, new_coords.x, new_coords.y, new_coords.z) < 20 * 20 && VEHICLE::GET_PED_IN_VEHICLE_SEAT(hnd, 0, true) == ent)
					break;
			}

			g_fiber_pool->queue_job([hnd] {
				auto ent = hnd;
				entity::take_control_of(ent);
				entity::delete_entity(ent);
			});

			std::erase_if(g.m_remote_player_teleports, [veh_id](auto& obj) {
				return obj.first == veh_id;
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
			g_notification_service.push_warning("Teleport", "Invalid Vehicle");

			return false;
		}

		int seat_index = 255;
		if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1, true))
			seat_index = -1;
		else if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -2, true))
			seat_index = -2;

		if (seat_index == 255)
		{
			g_notification_service.push_warning("Teleport", "There are no seats available in this vehicle for you.");

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

		if (sprite == (int)BlipIcons::RADAR_WAYPOINT)
			entity::load_ground_at_3dcoord(location);

		to_coords(location);

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

		to_coords(location);
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
		if (!to_blip((int)BlipIcons::RADAR_WAYPOINT))
		{
			g_notification_service.push_warning("Teleport", "No waypoint found.");

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
			g_notification_service.push_warning("Teleport", "No objective found.");
			return false;
		}

		to_coords(location);

		return true;
	}

	inline bool to_highlighted_blip()
	{
		if (!*g_pointers->m_gta.m_is_session_started)
		{
			g_notification_service.push_warning("Teleport", "You need to be online to use this feature.");
			return false;
		}

		auto blip = blip::get_selected_blip();
		if (blip == nullptr)
		{
			g_notification_service.push_warning("Teleport", "There is no blip selected on your map to teleport to.");
			return false;
		}
		Entity entity = self::ped;
		if (PED::GET_PED_CONFIG_FLAG(self::ped, 62, TRUE))
		{
			entity = self::veh;
		}
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, blip->m_position.x, blip->m_position.y, blip->m_position.z, FALSE, FALSE, TRUE);
		ENTITY::SET_ENTITY_HEADING(entity, blip->m_rotation);

		return false;
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
