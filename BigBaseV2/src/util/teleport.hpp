#pragma once
#include "blip.hpp"
#include "entity.hpp"
#include "notify.hpp"

namespace big::teleport
{
	inline bool bring_player(Player player)
	{
		Entity ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);

		if (ENTITY::IS_ENTITY_DEAD(ent, true))
		{
			notify::display_help_text("Target player is dead.");

			return false;
		}

		if (!PED::IS_PED_IN_ANY_VEHICLE(ent, true))
		{
			notify::display_help_text("Target player is not in a vehicle.");

			return false;
		}

		ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

		Vector3 location = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

		if (entity::take_control_of(ent))
			ENTITY::SET_ENTITY_COORDS(ent, location.x, location.y, location.z, 0, 0, 0, 0);
		else
			notify::display_help_text("Failed to take control of player vehicle.");

		return true;
	}

	inline bool godmode_kill(Player player)
	{
		Entity ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);

		if (ENTITY::IS_ENTITY_DEAD(ent, true))
		{
			notify::display_help_text("Target player is dead.");

			return false;
		}

		if (!PED::IS_PED_IN_ANY_VEHICLE(ent, true))
		{
			notify::display_help_text("Target player is not in a vehicle.");

			return false;
		}

		ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

		if (entity::take_control_of(ent))
			ENTITY::SET_ENTITY_COORDS(ent, 20000, 0, -99, 0, 0, 0, 0);
		else
			notify::display_help_text("Failed to take control of player vehicle.");

		return true;
	}

	inline bool load_ground_at_3dcoord(Vector3& location)
	{
		float groundZ;
		const uint8_t attempts = 10;

		for (uint8_t i = 0; i < attempts; i++)
		{
			// Only request a collision after the first try failed because the location might already be loaded on first attempt.
			for (uint16_t z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(location.x, location.y, (float)z);

				script::get_current()->yield();
			}

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, 1000.f, &groundZ, false, false))
			{
				location.z = groundZ + 1.f;

				return true;
			}

			script::get_current()->yield();
		}

		location.z = 1000.f;

		return false;
	}

	inline bool into_vehicle(Vehicle veh)
	{
		if (!veh)
		{
			notify::display_help_text("Player is not in a vehicle.");

			return false;
		}

		int seat_index = 255;
		if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1, true))
			seat_index = -1;
		else if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -2, true))
			seat_index = -2;

		if (seat_index == 255)
		{
			notify::display_help_text("No seats are free in the player vehicle.");

			return false;
		}

		Vector3 location = ENTITY::GET_ENTITY_COORDS(veh, true);
		load_ground_at_3dcoord(location);

		ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), location.x, location.y, location.z, 0, 0, 0, 0);

		script::get_current()->yield();

		PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, seat_index);

		return true;
	}

	inline void to_coords(Vector3 location)
	{
		PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), location.x, location.y, location.z + 1.f);
	}

	inline bool to_blip(int sprite, int color = -1)
	{
		Vector3 location;

		if (!blip::get_blip_location(location, sprite, color))
			return false;

		load_ground_at_3dcoord(location);

		PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), location.x, location.y, location.z);

		return true;
	}

	inline bool to_entity(Entity ent)
	{
		Vector3 location = ENTITY::GET_ENTITY_COORDS(ent, true);

		PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), location.x, location.y, location.z);

		return true;
	}

	inline bool parachute_to_entity(Entity ent)
	{
		Vector3 location = ENTITY::GET_ENTITY_COORDS(ent, true);

		WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 2, false, false); //gadget_parachute

		PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), location.x, location.y, location.z + 1000);

		return true;
	}

	inline bool to_player(Player player)
	{
		return to_entity(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
	}

	inline bool to_waypoint()
	{
		if (!to_blip((int)BlipIcons::Waypoint))
		{
			notify::above_map("Failed to find waypoint position");

			return false;
		}
		return true;
	}

	inline void Teleport_To_Appartment(Player target)
	{
		const size_t arg_count = 4;
		int args[4] = { (int)eRemoteEvent::Teleport , target, 1, 5 };

		g_pointers->m_trigger_script_event(1, args, arg_count, 1 << PLAYER::PLAYER_ID());
	}
}