/**
 * @file vehicle.hpp
 * @brief Basic vehicle related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "core/scr_globals.hpp"
#include "entity.hpp"
#include "gta/enums.hpp"
#include "gta/joaat.hpp"
#include "gta/vehicle_values.hpp"
#include "math.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "script_global.hpp"
#include "gta/enums.hpp"

namespace big::vehicle
{	struct disable_collisions
	{
		inline static memory::byte_patch* m_patch;
	};
	
	/**
	 * @brief Converts Miles Per Second into othether units.
	 * 
	 * @param mps Miles Per Second input.
	 * @param speed_unit Speed unit to convert to.
	 * @return Speed in specified units.
	 */
	inline float mps_to_speed(float mps, SpeedUnit speed_unit)
	{
		switch (speed_unit)
		{
		case SpeedUnit::KMPH: return mps * 3.6f; break;
		case SpeedUnit::MIPH: return mps * 2.2369f; break;
		}

		return mps;
	}

	/**
	 * @brief Converts specified units into Miles Per Second.
	 * 
	 * @param speed Speed input.
	 * @param speed_unit Speed unit to convert from.
	 * @return Speed in Miles Per Second.
	 */
	inline float speed_to_mps(float speed, SpeedUnit speed_unit)
	{
		switch (speed_unit)
		{
		case SpeedUnit::KMPH: return speed / 3.6f; break;
		case SpeedUnit::MIPH: return speed / 2.2369f; break;
		}

		return speed;
	}

	/**
	 * @brief Gets the offset to spawn vehicle at.
	 * @param spawn_inside Something...
	 * @param hash Vehicle hash to get better results.
	 * @param ped Entity to get offset from.
	 * @todo The Ped param should be entity instead.
	 * @return Position to spawn at.
	 */
	inline Vector3 get_spawn_location(bool spawn_inside, Hash hash, Ped ped = self::ped)
	{
		float y_offset = 0;

		if (self::veh != 0 && STREAMING::IS_MODEL_VALID(hash))
		{
			Vector3 min, max, result;
			MISC::GET_MODEL_DIMENSIONS(hash, &min, &max);
			result   = max - min;
			y_offset = result.y;
		}
		else if (!spawn_inside)
		{
			y_offset = 5.f;
		}

		return ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.f, y_offset, 0.f);
	}

	/**
	 * @brief Makes vehicle networked if it isn't.
	 * 
	 * @param veh Vehicle to set for.
	 * @param is_stolen Set's vehicle stolen flag.
	 */
	inline void set_mp_bitset(Vehicle veh, bool is_stolen = false)
	{
		DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
		auto networkId = NETWORK::VEH_TO_NET(veh);
		if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
			NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
		VEHICLE::SET_VEHICLE_IS_STOLEN(veh, is_stolen);
	}

	/**
	 * @brief Teleports vehicle to location.
	 * 
	 * @param veh Vehicle to teleport.
	 * @param location Location to teleport to.
	 * @param put_in Teleport local ped into vehicle.
	 * @param seatIdx Seat to teleport local ped into, -1 for any seat.
	 * @bug CLEAR_PED_TASKS_IMMEDIATELY not longer works with player peds.
	 */
	inline void bring(Vehicle veh, Vector3 location, bool put_in = true, int seatIdx = -1)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh))
			return g_notification_service->push_error("Vehicle", "Vehicle is not a valid one.");

		Vector3 vecVehicleLocation = ENTITY::GET_ENTITY_COORDS(veh, true);
		entity::load_ground_at_3dcoord(vecVehicleLocation);

		if (!entity::take_control_of(veh))
			return g_notification_service->push_warning("Vehicle", "Failed to take control of remote vehicle.");
		Ped ped = self::ped;

		ENTITY::SET_ENTITY_COORDS(veh, location.x, location.y, location.z + 1.f, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(ped));

		if (put_in)
		{
			for (size_t i = 0; i < 100 && math::distance_between_vectors(location, ENTITY::GET_ENTITY_COORDS(veh, true)) > 10; i++)
				script::get_current()->yield();

			auto driver_ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, false);

			if (driver_ped != 0)
			{
				if (PED::GET_PED_TYPE(driver_ped) == ePedType::PED_TYPE_NETWORK_PLAYER)
				{
					TASK::CLEAR_PED_TASKS_IMMEDIATELY(driver_ped);
				}
				else
				{
					entity::delete_entity(driver_ped);
				}
			}

			PED::SET_PED_INTO_VEHICLE(ped, veh, seatIdx);
		}
	}

	/**
	 * @brief Gets the closest vehicle to location using the replay interface.
	 * 
	 * @param location Location to search in.
	 * @param range Range to search in.
	 * @return Vehicle that has been found, if not found 0.
	 */
	inline Vehicle get_closest_to_location(Vector3 location, float range)
	{
		float min_dist   = FLT_MAX;
		int32_t m_handle = 0;

		for (const auto veh_entity : pools::get_all_vehicles())
		{
			const auto veh_ptr = veh_entity;
			if (!veh_ptr || !veh_ptr->m_navigation)
				continue;

			auto veh_pos_arr = *veh_ptr->m_navigation->get_position();
			Vector3 veh_pos(veh_pos_arr.x, veh_pos_arr.y, veh_pos_arr.z);

			float dist = math::distance_between_vectors(veh_pos, location);

			if (dist < min_dist)
			{
				int32_t tmp_handle = g_pointers->m_gta.m_ptr_to_handle(veh_ptr);

				if (entity::take_control_of(tmp_handle))
				{
					min_dist = dist;
					m_handle = tmp_handle;
				}
			}
		}

		return m_handle;
	}

	/**
	 * @brief Sets vehicles license plate text.
	 * 
	 * @param veh Vehicle to set license plate of.
	 * @param plate license plate text to set.
	 * @return True if set seccessfuly.
	 */
	inline bool set_plate(Vehicle veh, const char* plate)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh) || !entity::take_control_of(veh))
		{
			return false;
		}

		if (plate != nullptr && plate[0] != 0)
		{
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate);
		}

		return true;
	}

	/**
	 * @brief Fixes given vehicle.
	 * 
	 * @param veh Vehicle to fix.
	 * @return True if repaired seccessfuly. 
	 */
	inline bool repair(Vehicle veh)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh) || !entity::take_control_of(veh, 0))
		{
			return false;
		}

		VEHICLE::SET_VEHICLE_FIXED(veh);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.f);

		return true;
	}

	/**
	 * @brief Spawns vehicle.
	 * 
	 * @param hash Model hash to spawn.
	 * @param location Location to spawn vehicle on.
	 * @param heading The direction vehicle is facing.
	 * @param is_networked Is this vehicle synced to other players.
	 * @param script_veh Is this vehicle a script host vehicle.
	 * @param is_stolen Set's vehicle stolen flag.
	 * @return Spawned vehicle.
	 * @todo Better script_veh Description.
	 * @todo Check if VEHICLE::CREATE_VEHICLE p7 is related to YimMenu/YimMenu#424.
	 */
	inline Vehicle spawn(Hash hash, Vector3 location, float heading, bool is_networked = true, bool script_veh = false, bool is_stolen = false)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}

		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			return 0;
		}

		auto veh = VEHICLE::CREATE_VEHICLE(hash, location.x, location.y, location.z, heading, is_networked, script_veh, false);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		if (*g_pointers->m_gta.m_is_session_started)
		{
			set_mp_bitset(veh, is_stolen);
		}

		return veh;
	}

	inline Vehicle clone_from_vehicle_data(std::map<int, int32_t>& data, Vector3 location, float heading)
	{
		Vector3 tmpLocation = {location.x, location.y, 1200.0f};
		if (location.z > 1000.0f && location.z < 1400.0)
		{
			tmpLocation.z = 800.0f;
		}

		// vehicle data
		for (const auto& [idx, val] : data)
		{
			if (idx >= 0 && idx < 142)
			{
				*scr_globals::spawn_global.at(27).at(idx).as<int32_t*>() = val;
			}
		}

		// permission fix
		*scr_globals::spawn_global.at(27).at(1).as<int32_t*>() = 0;

		// personal car flag
		*scr_globals::spawn_global.at(27).at(94).as<int32_t*>() = 14;
		*scr_globals::spawn_global.at(27).at(95).as<int32_t*>() = 2;

		// mmi
		*scr_globals::spawn_global.at(27).at(103).as<int32_t*>() = 0;

		// spawn location
		*scr_globals::spawn_global.at(7).at(0).as<float*>() = tmpLocation.x;
		*scr_globals::spawn_global.at(7).at(1).as<float*>() = tmpLocation.y;
		*scr_globals::spawn_global.at(7).at(2).as<float*>() = tmpLocation.z;

		// spawn non pegasus
		*scr_globals::spawn_global.at(3).as<int*>() = 0;

		// spawn signal
		int* spawn_signal                               = scr_globals::spawn_global.at(2).as<int32_t*>();
		*scr_globals::spawn_global.at(5).as<int32_t*>() = 1;
		*spawn_signal                                   = 1;

		// wait until the vehicle is spawned
		for (size_t retry = 0; *spawn_signal != 0 && retry < 200; retry++)
		{
			script::get_current()->yield(10ms);
		}

		if (*spawn_signal == 1)
		{
			return 0;
		}

		auto veh = vehicle::get_closest_to_location(tmpLocation, 200);
		if (veh == 0)
		{
			return 0;
		}

		ENTITY::SET_ENTITY_COORDS(veh, location.x, location.y, location.z + 1.f, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_HEADING(veh, heading);

		return veh;
	}

	inline std::map<int, int32_t> get_vehicle_data_from_vehicle_idx(script_global vehicle_idx)
	{
		std::map<int, int32_t> veh_data;

		for (int i = 0; i < 142; i++)
		{
			veh_data[i] = *vehicle_idx.at(i).as<int32_t*>();
		}

		veh_data.erase(1);
		veh_data.erase(94);
		veh_data.erase(95);
		veh_data.erase(103);

		return veh_data;
	}

	inline std::map<int, int32_t> get_owned_mods_from_vehicle_idx(script_global vehicle_idx)
	{
		std::map<int, int32_t> owned_mods;

		for (int i = MOD_SECONDARY_CUSTOM; i <= MOD_MODEL_HASH; i++)
		{
			owned_mods[i] = 0;
		}

		int32_t val_32  = *vehicle_idx.at(32).as<int32_t*>();
		int32_t val_77  = *vehicle_idx.at(77).as<int32_t*>();
		int32_t val_102 = *vehicle_idx.at(102).as<int32_t*>();


		owned_mods[MOD_MODEL_HASH] = *vehicle_idx.at(66).as<int32_t*>();

		owned_mods[MOD_PLATE_STYLE] = *vehicle_idx.at(0).as<int32_t*>();
		owned_mods[MOD_WINDOW_TINT] = *vehicle_idx.at(65).as<int32_t*>();
		owned_mods[MOD_WHEEL_TYPE]  = *vehicle_idx.at(69).as<int32_t*>();


		owned_mods[MOD_PRIMARY_COL]     = *vehicle_idx.at(5).as<int32_t*>();
		owned_mods[MOD_SECONDARY_COL]   = *vehicle_idx.at(6).as<int32_t*>();
		owned_mods[MOD_PEARLESCENT_COL] = *vehicle_idx.at(7).as<int32_t*>();
		owned_mods[MOD_WHEEL_COL]       = *vehicle_idx.at(8).as<int32_t*>();
		owned_mods[MOD_INTERIOR_COL]    = *vehicle_idx.at(97).as<int32_t*>();
		owned_mods[MOD_DASHBOARD_COL]   = *vehicle_idx.at(99).as<int32_t*>();


		//CUSTOM PRIMARY
		owned_mods[MOD_PRIMARY_CUSTOM] = (val_77 & (1 << 13)) != 0;
		if (owned_mods[MOD_PRIMARY_CUSTOM])
		{
			owned_mods[MOD_PRIMARY_COL_R] = *vehicle_idx.at(71).as<int32_t*>();
			owned_mods[MOD_PRIMARY_COL_G] = *vehicle_idx.at(72).as<int32_t*>();
			owned_mods[MOD_PRIMARY_COL_B] = *vehicle_idx.at(73).as<int32_t*>();
		}


		//CUSTOM SECONDARY
		owned_mods[MOD_SECONDARY_CUSTOM] = (val_77 & (1 << 12)) != 0;
		if (owned_mods[MOD_SECONDARY_CUSTOM])
		{
			owned_mods[MOD_SECONDARY_COL_R] = *vehicle_idx.at(71).as<int32_t*>();
			owned_mods[MOD_SECONDARY_COL_G] = *vehicle_idx.at(72).as<int32_t*>();
			owned_mods[MOD_SECONDARY_COL_B] = *vehicle_idx.at(73).as<int32_t*>();
		}


		// TIRE SMOKE
		owned_mods[MOD_TIRESMOKE_COL_R] = *vehicle_idx.at(62).as<int32_t*>();
		owned_mods[MOD_TIRESMOKE_COL_G] = *vehicle_idx.at(63).as<int32_t*>();
		owned_mods[MOD_TIRESMOKE_COL_B] = *vehicle_idx.at(64).as<int32_t*>();
		owned_mods[MOD_TYRE_SMOKE] = !(owned_mods[MOD_TIRESMOKE_COL_R] == 255 && owned_mods[MOD_TIRESMOKE_COL_G] == 255 && owned_mods[MOD_TIRESMOKE_COL_B] == 255);


		// XENON
		if (val_32 > 0)
		{
			owned_mods[MOD_XENON_LIGHTS] = 1;
			owned_mods[MOD_XENON_COL]    = val_32 - 2;
		}
		else
		{
			owned_mods[MOD_XENON_LIGHTS] = 0;
		}


		// NEON
		owned_mods[MOD_NEON_LEFT_ON]  = (val_77 & (1 << 30)) != 0;
		owned_mods[MOD_NEON_RIGHT_ON] = (val_77 & (1 << 31)) != 0;
		owned_mods[MOD_NEON_FRONT_ON] = (val_77 & (1 << 28)) != 0;
		owned_mods[MOD_NEON_BACK_ON]  = (val_77 & (1 << 29)) != 0;
		owned_mods[MOD_NEON_COL_R]    = *vehicle_idx.at(74).as<int32_t*>();
		owned_mods[MOD_NEON_COL_G]    = *vehicle_idx.at(75).as<int32_t*>();
		owned_mods[MOD_NEON_COL_B]    = *vehicle_idx.at(76).as<int32_t*>();

		// TIRE OPTIONS
		owned_mods[MOD_TIRE_CAN_BURST] = (val_77 & (1 << 9)) == 0;
		if ((val_102 & 0b11) == 0b11)
		{
			owned_mods[MOD_DRIFT_TIRE] = 1;
		}

		owned_mods[MOD_TURBO] = *vehicle_idx.at(28).as<int32_t*>() != 0;

		owned_mods[MOD_FRONTWHEEL_VAR] = *vehicle_idx.at(60).as<int32_t*>() != 0;
		owned_mods[MOD_REARWHEEL_VAR]  = *vehicle_idx.at(61).as<int32_t*>() != 0;


		// OTHER MODS
		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			if (slot == MOD_TURBO || slot == MOD_TYRE_SMOKE || slot == MOD_XENON_LIGHTS)
			{
				continue;
			}

			int32_t val = *vehicle_idx.at(10 + slot).as<int32_t*>() - 1;
			if (val != -1)
			{
				owned_mods[slot] = val;
			}
		}

		// EXTRA
		for (int extra = MOD_EXTRA_9; extra <= MOD_EXTRA_1; extra++)
		{
			int gta_extra_id  = (extra - MOD_EXTRA_0) * -1;
			owned_mods[extra] = val_77 >> (gta_extra_id - 1) & 1;
		}

		return owned_mods;
	}


	inline Vehicle clone_from_owned_mods(std::map<int, int32_t> owned_mods, Vector3 location, float heading, bool is_networked = true)
	{
		auto vehicle = spawn(owned_mods[MOD_MODEL_HASH], location, heading, is_networked);
		if (vehicle == 0)
		{
			return 0;
		}

		for (int i = MOD_NEON_COL_B; i <= MOD_MODEL_HASH; i++)
		{
			if (owned_mods.count(i) == 0)
			{
				owned_mods[i] = 0;
			}
		}

		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
		script::get_current()->yield(10ms);

		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, owned_mods[MOD_PLATE_STYLE]);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, owned_mods[MOD_WINDOW_TINT]);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, owned_mods[MOD_WHEEL_TYPE]);
		script::get_current()->yield(10ms);

		VEHICLE::SET_VEHICLE_COLOURS(vehicle, owned_mods[MOD_PRIMARY_COL], owned_mods[MOD_SECONDARY_COL]);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, owned_mods[MOD_PEARLESCENT_COL], owned_mods[MOD_WHEEL_COL]);
		VEHICLE::SET_VEHICLE_EXTRA_COLOUR_5(vehicle, owned_mods[MOD_INTERIOR_COL]);
		VEHICLE::SET_VEHICLE_EXTRA_COLOUR_6(vehicle, owned_mods[MOD_DASHBOARD_COL]);

		if (owned_mods[MOD_PRIMARY_CUSTOM])
		{
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, owned_mods[MOD_PRIMARY_COL_R], owned_mods[MOD_PRIMARY_COL_G], owned_mods[MOD_PRIMARY_COL_B]);
		}

		if (owned_mods[MOD_SECONDARY_CUSTOM])
		{
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, owned_mods[MOD_SECONDARY_COL_R], owned_mods[MOD_SECONDARY_COL_G], owned_mods[MOD_SECONDARY_COL_B]);
		}

		if (owned_mods[MOD_TYRE_SMOKE])
		{
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, owned_mods[MOD_TIRESMOKE_COL_R], owned_mods[MOD_TIRESMOKE_COL_G], owned_mods[MOD_TIRESMOKE_COL_B]);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TYRE_SMOKE, owned_mods[MOD_TYRE_SMOKE]);
		}

		if (owned_mods[MOD_XENON_LIGHTS])
		{
			VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(vehicle, owned_mods[MOD_XENON_COL]);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_XENON_LIGHTS, owned_mods[MOD_XENON_LIGHTS]);
		}

		VEHICLE::SET_VEHICLE_NEON_COLOUR(vehicle, owned_mods[MOD_NEON_COL_R], owned_mods[MOD_NEON_COL_G], owned_mods[MOD_NEON_COL_B]);
		VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, NEON_LEFT, owned_mods[MOD_NEON_LEFT_ON]);
		VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, NEON_RIGHT, owned_mods[MOD_NEON_RIGHT_ON]);
		VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, NEON_FRONT, owned_mods[MOD_NEON_FRONT_ON]);
		VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, NEON_BACK, owned_mods[MOD_NEON_BACK_ON]);


		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, owned_mods[MOD_TIRE_CAN_BURST]);
		VEHICLE::SET_DRIFT_TYRES(vehicle, owned_mods[MOD_DRIFT_TIRE]);
		VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TURBO, owned_mods[MOD_TURBO]);

		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			if (owned_mods.count(slot) && owned_mods[slot] != -1)
			{
				bool custom_tire = false;

				if (slot == MOD_FRONTWHEEL)
				{
					custom_tire = owned_mods[MOD_FRONTWHEEL_VAR];
				}
				else if (slot == MOD_REARWHEEL)
				{
					custom_tire = owned_mods[MOD_REARWHEEL_VAR];
				}

				VEHICLE::SET_VEHICLE_MOD(vehicle, slot, owned_mods[slot], custom_tire);
			}
		}

		for (int extra = MOD_EXTRA_14; extra <= MOD_EXTRA_1; extra++)
		{
			int gta_extra_id = (extra - MOD_EXTRA_0) * -1;
			if (owned_mods.count(extra) && VEHICLE::DOES_EXTRA_EXIST(vehicle, gta_extra_id))
			{
				VEHICLE::SET_VEHICLE_EXTRA(vehicle, gta_extra_id, owned_mods[extra] == 0);
			}
		}

		return vehicle;
	}

	inline std::map<int, int32_t> get_owned_mods_from_vehicle(Vehicle vehicle)
	{
		std::map<int, int32_t> owned_mods;

		for (int i = MOD_SECONDARY_CUSTOM; i <= MOD_MODEL_HASH; i++)
		{
			owned_mods[i] = 0;
		}

		owned_mods[MOD_MODEL_HASH] = ENTITY::GET_ENTITY_MODEL(vehicle);

		owned_mods[MOD_PLATE_STYLE] = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle);
		owned_mods[MOD_WINDOW_TINT] = VEHICLE::GET_VEHICLE_WINDOW_TINT(vehicle);
		owned_mods[MOD_WHEEL_TYPE]  = VEHICLE::GET_VEHICLE_WHEEL_TYPE(vehicle);

		VEHICLE::GET_VEHICLE_COLOURS(vehicle, &owned_mods[MOD_PRIMARY_COL], &owned_mods[MOD_SECONDARY_COL]);
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &owned_mods[MOD_PEARLESCENT_COL], &owned_mods[MOD_WHEEL_COL]);
		VEHICLE::GET_VEHICLE_EXTRA_COLOUR_5(vehicle, &owned_mods[MOD_INTERIOR_COL]);
		VEHICLE::GET_VEHICLE_EXTRA_COLOUR_6(vehicle, &owned_mods[MOD_DASHBOARD_COL]);

		owned_mods[MOD_PRIMARY_CUSTOM] = VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(vehicle);
		if (owned_mods[MOD_PRIMARY_CUSTOM])
		{
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, &owned_mods[MOD_PRIMARY_COL_R], &owned_mods[MOD_PRIMARY_COL_G], &owned_mods[MOD_PRIMARY_COL_B]);
		}

		owned_mods[MOD_SECONDARY_CUSTOM] = VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(vehicle);
		if (owned_mods[MOD_SECONDARY_CUSTOM])
		{
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, &owned_mods[MOD_SECONDARY_COL_R], &owned_mods[MOD_SECONDARY_COL_G], &owned_mods[MOD_SECONDARY_COL_B]);
		}

		owned_mods[MOD_TYRE_SMOKE] = VEHICLE::IS_TOGGLE_MOD_ON(vehicle, MOD_TYRE_SMOKE);
		if (owned_mods[MOD_TYRE_SMOKE])
		{
			VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, &owned_mods[MOD_TIRESMOKE_COL_R], &owned_mods[MOD_TIRESMOKE_COL_G], &owned_mods[MOD_TIRESMOKE_COL_B]);
		}

		owned_mods[MOD_XENON_LIGHTS] = VEHICLE::IS_TOGGLE_MOD_ON(vehicle, MOD_XENON_LIGHTS);
		if (owned_mods[MOD_XENON_LIGHTS])
		{
			owned_mods[MOD_XENON_COL] = (int8_t)VEHICLE::GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(vehicle);
		}

		owned_mods[MOD_NEON_LEFT_ON]  = VEHICLE::GET_VEHICLE_NEON_ENABLED(vehicle, NEON_LEFT);
		owned_mods[MOD_NEON_RIGHT_ON] = VEHICLE::GET_VEHICLE_NEON_ENABLED(vehicle, NEON_RIGHT);
		owned_mods[MOD_NEON_FRONT_ON] = VEHICLE::GET_VEHICLE_NEON_ENABLED(vehicle, NEON_FRONT);
		owned_mods[MOD_NEON_BACK_ON]  = VEHICLE::GET_VEHICLE_NEON_ENABLED(vehicle, NEON_BACK);
		VEHICLE::GET_VEHICLE_NEON_COLOUR(vehicle, &owned_mods[MOD_NEON_COL_R], &owned_mods[MOD_NEON_COL_G], &owned_mods[MOD_NEON_COL_B]);

		owned_mods[MOD_TIRE_CAN_BURST] = VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(vehicle);
		owned_mods[MOD_DRIFT_TIRE]     = VEHICLE::GET_DRIFT_TYRES_SET(vehicle);
		owned_mods[MOD_TURBO]          = VEHICLE::IS_TOGGLE_MOD_ON(vehicle, MOD_TURBO);

		owned_mods[MOD_FRONTWHEEL_VAR] = VEHICLE::GET_VEHICLE_MOD_VARIATION(vehicle, MOD_FRONTWHEEL);
		owned_mods[MOD_REARWHEEL_VAR]  = VEHICLE::GET_VEHICLE_MOD_VARIATION(vehicle, MOD_REARWHEEL);

		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			int count = VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, slot);
			if (count > 0)
			{
				int32_t val = VEHICLE::GET_VEHICLE_MOD(vehicle, slot);

				if (val != -1)
				{
					owned_mods[slot] = val;
				}
			}
		}

		for (int extra = MOD_EXTRA_14; extra <= MOD_EXTRA_1; extra++)
		{
			int gta_extra_id = (extra - MOD_EXTRA_0) * -1;
			if (VEHICLE::DOES_EXTRA_EXIST(vehicle, gta_extra_id))
			{
				owned_mods[extra] = VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, gta_extra_id);
			}
		}

		return owned_mods;
	}

	inline void teleport_into_vehicle(Vehicle veh)
	{
		PED::SET_PED_INTO_VEHICLE(self::ped, veh, -1);
	}

	inline void max_vehicle(Vehicle veh)
	{
		Hash model = ENTITY::GET_ENTITY_MODEL(veh);

		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TURBO, TRUE);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TYRE_SMOKE, TRUE);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_XENON_LIGHTS, TRUE);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 1);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, false);

		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			if (slot == MOD_LIVERY)
			{
				continue;
			}

			int count = VEHICLE::GET_NUM_VEHICLE_MODS(veh, slot);
			if (count > 0)
			{
				int selected_mod = -1;

				for (int mod = count - 1; mod >= -1; mod--)
				{
					selected_mod = mod;
					break;
				}

				if (selected_mod != -1)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, slot, selected_mod, true);
				}
			}
		}
	}

	/**
	 * @brief Upgrades vehicle to max without doing visual modifications.
	 *
	 * @param veh Vehicle to upgrade.
	 */
	inline void max_vehicle_performance(Vehicle veh)
	{
		if (entity::take_control_of(veh))
		{
			VehicleModType perfomance_mods[] = {MOD_ENGINE, MOD_BRAKES, MOD_TRANSMISSION, MOD_SUSPENSION, MOD_ARMOR, MOD_NITROUS, MOD_TURBO};
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

			for (auto mod_slot : perfomance_mods)
			{
				if (mod_slot != MOD_NITROUS && mod_slot != MOD_TURBO)
					VEHICLE::SET_VEHICLE_MOD(veh, mod_slot, VEHICLE::GET_NUM_VEHICLE_MODS(veh, mod_slot) - 1, true);
				else
					VEHICLE::TOGGLE_VEHICLE_MOD(veh, mod_slot, true);
			}
		}
	}

	/**
	 * @brief Sets vehicle engine state.
	 * 
	 * @param current_vehicle Vehicle to set the engine state of.
	 * @param state Engine state, true on, false off.
	 * @param immediately Don't play start up animation and sound.
	 * @param disable_auto_start If we have torned off the engine this will prevent local ped from starting the back automatically.
	 * @bug Does not check for control.
	 */
	inline void set_engine_state(Vehicle current_vehicle, bool state, bool immediately, bool disable_auto_start)
	{
		if (current_vehicle)
			VEHICLE::SET_VEHICLE_ENGINE_ON(current_vehicle, state, immediately, disable_auto_start);
		else
			return g_notification_service->push_warning("Vehicle", "Please enter a vehicle.");
	}

	/**
	 * @brief Removes all modifactions from given vehicle.
	 * Similar to `downgradeveh` command.
	 * 
	 * @param vehicle Vehicle to downgrade.
	 */
	inline void downgrade(Vehicle vehicle)
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
		for (int i = 0; i < 50; i++)
		{
			VEHICLE::REMOVE_VEHICLE_MOD(vehicle, i);
		}
	}

	inline bool remote_control_vehicle(Vehicle veh)
	{
		if (!entity::take_control_of(veh, 4000))
		{
			g_notification_service->push_warning("Remote Control", "Failed to take control of remote vehicle.");
			return false;
		}

		if (g.m_remote_controlled_vehicle == veh)
		{
			return false;
		}

		Hash model      = ENTITY::GET_ENTITY_MODEL(veh);
		Vehicle spawned = vehicle::spawn(model, self::pos, 0.0f);

		ENTITY::SET_ENTITY_ALPHA(spawned, 0, FALSE);
		if (!VEHICLE::IS_THIS_MODEL_A_BIKE(model))
			ENTITY::SET_ENTITY_VISIBLE(spawned, FALSE, FALSE);
		ENTITY::SET_ENTITY_INVINCIBLE(spawned, TRUE);

		float heading    = ENTITY::GET_ENTITY_HEADING(veh);
		Vector3 rotation = ENTITY::GET_ENTITY_ROTATION(veh, 2);
		Vector3 coords   = ENTITY::GET_ENTITY_COORDS(veh, FALSE);
		Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(veh);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(spawned, coords.x, coords.y, coords.z, FALSE, FALSE, FALSE);
		ENTITY::SET_ENTITY_HEADING(spawned, heading);
		ENTITY::SET_ENTITY_ROTATION(spawned, rotation.x, rotation.y, rotation.z, 2, TRUE);

		ENTITY::SET_ENTITY_VISIBLE(veh, TRUE, FALSE);

		ENTITY::SET_ENTITY_COLLISION(veh, FALSE, FALSE);
		ENTITY::SET_ENTITY_INVINCIBLE(veh, TRUE);
		VEHICLE::SET_VEHICLE_DOORS_LOCKED(veh, 4);
		VEHICLE::SET_VEHICLE_MAX_SPEED(veh, 0.0001f);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(veh, spawned, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, FALSE, FALSE, FALSE, FALSE, 0, TRUE, FALSE);
		PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), spawned, -1);

		VEHICLE::SET_VEHICLE_ENGINE_ON(spawned, TRUE, TRUE, FALSE);
		ENTITY::SET_ENTITY_VELOCITY(spawned, velocity.x, velocity.y, velocity.z);
		VEHICLE::COPY_VEHICLE_DAMAGES(veh, spawned);

		g.m_remote_controller_vehicle = spawned;
		g.m_remote_controlled_vehicle = veh;
		return true;
	}

	inline const char* get_mod_slot_name(Hash model, Vehicle vehicle, int mod_slot)
	{
		switch (mod_slot)
		{
		case MOD_HOOD: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_HOD");
		case MOD_ARMOR: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_ARM");
		case MOD_BRAKES: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_BRA");
		case MOD_ENGINE: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_ENG");
		case MOD_SUSPENSION: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_SUS");
		case MOD_TRANSMISSION: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_TRN");
		case MOD_HORNS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_HRN");
		case MOD_FRONTWHEEL:
			if (!VEHICLE::IS_THIS_MODEL_A_BIKE(model) && VEHICLE::IS_THIS_MODEL_A_BICYCLE(model))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_MOD_WHEM");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_WHE0_0");
		case MOD_REARWHEEL:
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_WHE0_1");
			//Bennys
		case MOD_PLATEHOLDER: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S0");
		case MOD_VANITYPLATES: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S1");
		case MOD_TRIMDESIGN:
			if (model == RAGE_JOAAT("sultanrs"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S2b");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S2");
		case MOD_ORNAMENTS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S3");
		case MOD_DASHBOARD: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S4");
		case MOD_DIALDESIGN: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S5");
		case MOD_DOORSPEAKERS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S6");
		case MOD_SEATS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S7");
		case MOD_STEERINGWHEELS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S8");
		case MOD_COLUMNSHIFTERLEVERS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S9");
		case MOD_PLAQUES: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S10");
		case MOD_SPEAKERS: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S11");
		case MOD_TRUNK: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S12");
		case MOD_HYDRO: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S13");
		case MOD_ENGINEBLOCK: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S14");
		case MOD_AIRFILTER:
			if (model == RAGE_JOAAT("sultanrs"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S15b");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S15");
		case MOD_STRUTS:
			if (model == RAGE_JOAAT("sultanrs") || model == RAGE_JOAAT("banshee2"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S16b");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S16");
		case MOD_ARCHCOVER:
			if (model == RAGE_JOAAT("sultanrs"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S17b");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S17");
		case MOD_AERIALS:
			if (model == RAGE_JOAAT("sultanrs"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S18b");
			else if (model == RAGE_JOAAT("btype3"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S18c");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S18");
		case MOD_TRIM:
			if (model == RAGE_JOAAT("sultanrs"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S19b");
			else if (model == RAGE_JOAAT("btype3"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S19c");
			else if (model == RAGE_JOAAT("virgo2"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S19d");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S19");
		case MOD_TANK:
			if (model == RAGE_JOAAT("slamvan3"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S27");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S20");
		case MOD_WINDOWS:
			if (model == RAGE_JOAAT("btype3"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S21b");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S21");
		case MOD_DOORS:
			if (model == RAGE_JOAAT("slamvan3"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("SLVAN3_RDOOR");
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S22");
		case MOD_LIVERY: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMM_MOD_S23");
		default:
			auto name = VEHICLE::GET_MOD_SLOT_NAME(vehicle, mod_slot);
			if (name == nullptr)
				return "";
			if (strstr(name, "_"))
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(name);
			return name;
		}
	}

	static const std::map<int, const char*> horn_map = {{-1, "CMOD_HRN_0"},
	    {0, "CMOD_HRN_TRK"},
	    {1, "CMOD_HRN_COP"},
	    {2, "CMOD_HRN_CLO"},
	    {3, "CMOD_HRN_MUS1"},
	    {4, "CMOD_HRN_MUS2"},
	    {5, "CMOD_HRN_MUS3"},
	    {6, "CMOD_HRN_MUS4"},
	    {7, "CMOD_HRN_MUS5"},
	    {8, "CMOD_HRN_SAD"},
	    {9, "HORN_CLAS1"},
	    {10, "HORN_CLAS2"},
	    {11, "HORN_CLAS3"},
	    {12, "HORN_CLAS4"},
	    {13, "HORN_CLAS5"},
	    {14, "HORN_CLAS6"},
	    {15, "HORN_CLAS7"},
	    {16, "HORN_CNOTE_C0"},
	    {17, "HORN_CNOTE_D0"},
	    {18, "HORN_CNOTE_E0"},
	    {19, "HORN_CNOTE_F0"},
	    {20, "HORN_CNOTE_G0"},
	    {21, "HORN_CNOTE_A0"},
	    {22, "HORN_CNOTE_B0"},
	    {23, "HORN_CNOTE_C1"},
	    {24, "HORN_HIPS1"},
	    {25, "HORN_HIPS2"},
	    {26, "HORN_HIPS3"},
	    {27, "HORN_HIPS4"},
	    {28, "HORN_INDI_1"},
	    {29, "HORN_INDI_2"},
	    {30, "HORN_INDI_3"},
	    {31, "HORN_INDI_4"},
	    {32, "HORN_LUXE2"},
	    {33, "HORN_LUXE1"},
	    {34, "HORN_LUXE3"}, /*{35, "HORN_LUXE2"},
		{36, "HORN_LUXE1"}, {37, "HORN_LUXE3"},*/
	    {38, "HORN_HWEEN1"},
	    /*{39, "HORN_HWEEN1"},*/ {40, "HORN_HWEEN2"},
	    /*{41, "HORN_HWEEN2"},*/ {42, "HORN_LOWRDER1"},
	    /*{43, "HORN_LOWRDER1"},*/ {44, "HORN_LOWRDER2"}, /*{45, "HORN_LOWRDER2"},*/
	    {46, "HORN_XM15_1"},
	    {47, "HORN_XM15_2"},
	    {48, "HORN_XM15_3"}};

	inline const char* get_mod_name(Hash model, Vehicle vehicle, int mod_slot, int mod, int mod_count)
	{
		if (mod_count == 0)
			return "";
		if (mod < -1 || mod >= mod_count)
			return "";
		if (mod_slot == MOD_HORNS)
		{
			if (horn_map.find(mod) != horn_map.end())
			{
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(horn_map.find(mod)->second);
			}
			return "";
		}

		if (mod_slot == MOD_FRONTWHEEL || mod_slot == MOD_REARWHEEL)
		{
			if (mod == -1)
			{
				if (!VEHICLE::IS_THIS_MODEL_A_BIKE(model) && VEHICLE::IS_THIS_MODEL_A_BICYCLE(model))
					return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_WHE_0");
				else
					return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_WHE_B_0");
			}
			if (mod >= mod_count / 2)
				//return std::format("{} {}", HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("Chrome"), HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MOD_TEXT_LABEL(vehicle, mod_slot, mod))).c_str(); //Bug with FMT library? Returns Chrome Chrome...
				return std::format("Chrome {}", HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MOD_TEXT_LABEL(vehicle, mod_slot, mod)))
				    .c_str();
			else
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MOD_TEXT_LABEL(vehicle, mod_slot, mod));
		}

		switch (mod_slot)
		{
		case MOD_ARMOR: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(std::format("CMOD_ARM_{}", (mod + 1)).c_str());
		case MOD_BRAKES: return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(std::format("CMOD_BRA_{}", (mod + 1)).c_str());
		case MOD_ENGINE:
			if (mod == -1)
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_ARM_0");
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(std::format("CMOD_ENG_{}", (mod + 1)).c_str());
		case MOD_SUSPENSION:
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(std::format("CMOD_SUS_{}", (mod + 1)).c_str());
		case MOD_TRANSMISSION:
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(std::format("CMOD_GBX_{}", (mod + 1)).c_str());
		}


		if (mod > -1)
		{
			if (mod_slot == MOD_SIDESKIRT && VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, MOD_SIDESKIRT) < 2)
			{
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_SKI_1");
			}
			auto label = VEHICLE::GET_MOD_TEXT_LABEL(vehicle, mod_slot, mod);
			if (label == nullptr || strlen(label) == 0)
				return "MISSING_LABEL";
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(label);
		}
		else
		{
			switch (mod_slot)
			{
			case MOD_AIRFILTER: break;
			case MOD_STRUTS:
				switch (model)
				{
				case RAGE_JOAAT("banshee"):
				case RAGE_JOAAT("banshee2"):
				case RAGE_JOAAT("sultanrs"): return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_COL5_41");
				}
				break;
			}
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION("CMOD_DEF_0");
		}
	}

	/*
	 Set doorId to eDoorId::VEH_EXT_DOOR_INVALID_ID or simply -1 to apply to all vehicle doors.
	*/
	inline bool change_vehicle_door_lock_state(Vehicle veh, eDoorId doorId, eVehicleLockState state)
	{
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			if (doorId == eDoorId::VEH_EXT_DOOR_INVALID_ID)
			{
				VEHICLE::SET_VEHICLE_DOORS_LOCKED(veh, (int)state);
				for (int i = 0; i < 6; i++)
					VEHICLE::SET_VEHICLE_INDIVIDUAL_DOORS_LOCKED(veh, i, (int)state);
				return VEHICLE::GET_VEHICLE_DOOR_LOCK_STATUS(veh) == (int)state;
			}
			else
			{
				if (VEHICLE::GET_IS_DOOR_VALID(veh, (int)doorId))
					VEHICLE::SET_VEHICLE_INDIVIDUAL_DOORS_LOCKED(veh, (int)doorId, (int)state);

				return VEHICLE::GET_VEHICLE_INDIVIDUAL_DOOR_LOCK_STATUS(veh, (int)doorId) == (int)state;
			}
		}

		return false;
	}

	/*
	* Set 'open' to false to close the door.
	* Set doorId to eDoorId::VEH_EXT_DOOR_INVALID_ID or simply -1 to apply to all doors.
	*/
	inline bool operate_vehicle_door(Vehicle veh, eDoorId doorId, bool open)
	{
		bool success = false;
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			for (int i = 0; i < 6; i++)
			{
				if (doorId == eDoorId::VEH_EXT_DOOR_INVALID_ID || (int)doorId == i)
				{
					if (VEHICLE::GET_IS_DOOR_VALID(veh, i))
					{
						if (open)
							VEHICLE::SET_VEHICLE_DOOR_OPEN(veh, i, false, false);
						else
							VEHICLE::SET_VEHICLE_DOOR_SHUT(veh, i, false);
					}
					success = true;
				}
			}
		}
		return success;
	}

	inline bool operate_vehicle_window(Vehicle veh, eWindowId windowId, bool open)
	{
		bool success = false;
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			for (int i = 0; i < 4; i++)
			{
				if (windowId == eWindowId::WINDOW_INVALID_ID)
				{
					if (open)
						VEHICLE::ROLL_DOWN_WINDOWS(veh);
					else
						VEHICLE::ROLL_UP_WINDOW(veh, i);
				}

				if ((int)windowId == i)
				{
					if (open)
						VEHICLE::ROLL_DOWN_WINDOW(veh, i);
					else
						VEHICLE::ROLL_UP_WINDOW(veh, i);

					success = true;
				}
			}
		}
		return success;
	}

	inline bool operate_vehicle_headlights(Vehicle veh, bool lights, bool highbeams)
	{
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			VEHICLE::SET_VEHICLE_FULLBEAM(veh, highbeams);
			VEHICLE::SET_VEHICLE_LIGHTS(veh, lights ? 3 : 4);
			int regular, highbeam;
			VEHICLE::GET_VEHICLE_LIGHTS_STATE(veh, &regular, &highbeam);
			return regular == (int)lights && (int)highbeams == highbeam;
		}

		return false;
	}

	/*
	* Input index -1 to apply to all neons.
	*/
	inline bool operate_vehicle_neons(Vehicle veh, int index, bool toggle)
	{
		bool success = false;
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
			for (int i = 0; i < 4; i++)
			{
				if (index == -1 || index == i)
				{
					VEHICLE::SET_VEHICLE_NEON_ENABLED(veh, index, toggle);
					success = true;
				}
			}
		}

		return success;
	}
}
