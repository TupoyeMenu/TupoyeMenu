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
#include "gta/enums.hpp"
#include "memory/byte_patch.hpp"
#include "script_global.hpp"

namespace big::vehicle
{
	struct disable_collisions
	{
		inline static memory::byte_patch* m_patch;
	};

	float mps_to_speed(float mps, SpeedUnit speed_unit);
	float speed_to_mps(float speed, SpeedUnit speed_unit);
	Vector3 get_spawn_location(bool spawn_inside, Hash hash, Ped ped = self::ped);
	void set_mp_bitset(Vehicle veh);
	void bring(Vehicle veh, Vector3 location, bool put_in = true, int seatIdx = -1);
	Vehicle get_closest_to_location(Vector3 location, float range);
	bool set_plate(Vehicle veh, const char* plate);
	bool repair(Vehicle veh);
	Vehicle spawn(Hash hash, Vector3 location, float heading, bool is_networked = true, bool script_veh = false, bool is_stolen = false);
	Vehicle clone_from_vehicle_data(std::map<int, int32_t>& data, Vector3 location, float heading);
	std::map<int, int32_t> get_owned_mods_from_vehicle_idx(script_global vehicle_idx);
	Vehicle clone_from_owned_mods(std::map<int, int32_t> owned_mods, Vector3 location, float heading, bool is_networked = true);
	std::map<int, int32_t> get_owned_mods_from_vehicle(Vehicle vehicle);
	void teleport_into_vehicle(Vehicle veh);
	void max_vehicle(Vehicle veh);
	void max_vehicle_performance(Vehicle veh);
	void set_engine_state(Vehicle current_vehicle, bool state, bool immediately, bool disable_auto_start);
	void downgrade(Vehicle vehicle);
	bool remote_control_vehicle(Vehicle veh);
	const char* get_mod_name(Hash model, Vehicle vehicle, int mod_slot, int mod, int mod_count);
	const char* get_mod_slot_name(Hash model, Vehicle vehicle, int mod_slot);
	extern const std::map<int, const char*> horn_map;
	
	/*
	 Set doorId to eDoorId::VEH_EXT_DOOR_INVALID_ID or simply -1 to apply to all vehicle doors.
	*/
	bool change_vehicle_door_lock_state(Vehicle veh, eDoorId doorId, eVehicleLockState state);

	/*
	* Set 'open' to false to close the door.
	* Set doorId to eDoorId::VEH_EXT_DOOR_INVALID_ID or simply -1 to apply to all doors.
	*/
	bool operate_vehicle_door(Vehicle veh, eDoorId doorId, bool open);
	bool operate_vehicle_window(Vehicle veh, eWindowId windowId, bool open);

	bool operate_vehicle_headlights(Vehicle veh, bool lights, bool highbeams);

	/*
	* Input index -1 to apply to all neons.
	*/
	bool operate_vehicle_neons(Vehicle veh, int index, bool toggle);
}
