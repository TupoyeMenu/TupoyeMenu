#pragma once
#include "gta/joaat.hpp"
#include "gta/replay.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "services/object_spooner/object_spooner_service.hpp"
#include "math.hpp"

namespace big::entity
{
	inline bool take_control_of(Entity ent, int timeout = 300)
	{
		if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent) || !*g_pointers->m_is_session_started) 
			return true;

		for (int i = 0; !NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent) && i < timeout; i++)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(ent);

			if (timeout != 0)
				script::get_current()->yield();
		}

		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent)) 
			return false;

		int netHandle = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(ent);
		NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netHandle, true);

		return true;
	}

	inline void cage_ped(Ped ped)
	{
		Hash hash = RAGE_JOAAT("prop_gold_cont_01");

		Vector3 location = ENTITY::GET_ENTITY_COORDS(ped, true);
		OBJECT::CREATE_OBJECT(hash, location.x, location.y, location.z - 1.f, true, false, false);
	}

	inline void clean_ped(Ped ped)
	{
		Ped player_ped = self::ped;

		PED::CLEAR_PED_BLOOD_DAMAGE(player_ped);
		PED::CLEAR_PED_WETNESS(player_ped);
		PED::CLEAR_PED_ENV_DIRT(player_ped);
		PED::RESET_PED_VISIBLE_DAMAGE(player_ped);
	}

	inline void delete_entity(Entity ent)
	{
		if(take_control_of(ent, 1000))
		{
			ENTITY::DETACH_ENTITY(ent, 1, 1);
			ENTITY::SET_ENTITY_VISIBLE(ent, false, false);
			NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(ent, true);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, 50'000, 50'000, 100'000, 0, 0, 0);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ent, 1, 1);
			ENTITY::DELETE_ENTITY(&ent);
		}
		else
		{
			g_notification_service->push_warning("Delete Crash Ped", "Delete entity failed.");
		}
	}

	inline bool raycast(Entity* ent)
	{
		BOOL hit;
		Vector3 endCoords;
		Vector3 surfaceNormal;

		Vector3 camCoords = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		Vector3 dir = math::rotation_to_direction(rot);
		Vector3 farCoords;

		farCoords.x = camCoords.x + dir.x * 1000;
		farCoords.y = camCoords.y + dir.y * 1000;
		farCoords.z = camCoords.z + dir.z * 1000;

		int ray = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(camCoords.x, camCoords.y, camCoords.z, farCoords.x, farCoords.y, farCoords.z, -1, 0, 7);
		SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, &endCoords, &surfaceNormal, ent);

		return (bool)hit;
	}

	inline bool raycast(Entity* ent, Vector3* endCoords, Vector3* surfaceNormal, Vector3 camCoords, Vector3 direction, Entity ignore_ent = 0)
	{
		BOOL hit;
		Vector3 farCoords;

		farCoords.x = camCoords.x + direction.x * 1000;
		farCoords.y = camCoords.y + direction.y * 1000;
		farCoords.z = camCoords.z + direction.z * 1000;

		int ray = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(camCoords.x, camCoords.y, camCoords.z, farCoords.x, farCoords.y, farCoords.z, 511, ignore_ent, 0);
		SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, endCoords, surfaceNormal, ent);

		return (bool)hit;
	}

	inline bool raycast_mouse(Entity* ent, Vector3* startCoords, Vector3* endCoords, Vector3* surfaceNormal, Entity ignore_ent = 0)
	{
		BOOL hit;

		int ray = SHAPETEST::START_SHAPE_TEST_MOUSE_CURSOR_LOS_PROBE(startCoords, endCoords, 511, ignore_ent, 0);
		SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, endCoords, surfaceNormal, ent);

		return (bool)hit;
	}
}