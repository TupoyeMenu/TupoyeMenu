#include "backend/looped/looped.hpp"
#include "gta/enums.hpp"
#include "natives.hpp"
#include "util/entity.hpp"

namespace big
{
	//menyoo horn boost
	void looped::vehicle_flares()
	{
		if (!g.vehicle.flares) return;

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Hash hash = MISC::GET_HASH_KEY("weapon_flaregun");
			STREAMING::REQUEST_MODEL(hash);
			WEAPON::REQUEST_WEAPON_ASSET(hash, 31, 26);
			if (PAD::IS_CONTROL_JUST_RELEASED(0, 355))
			{

				auto offset1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, -6.0, -4.0, -0.2);
				auto offset2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, -3.0, -4.0, -0.2);
				auto offset3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 6.0, -4.0, -0.2);
				auto offset4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 3.0, -4.0, -0.2);
				AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "flares_released", vehicle, "DLC_SM_Countermeasures_Sounds", true, 0);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset1.x, offset1.y, offset1.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false, false);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset1.x, offset1.y, offset1.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false, false);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset2.x, offset2.y, offset2.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false, false);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset3.x, offset3.y, offset3.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false, false);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset4.x, offset4.y, offset4.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false, false);
			}
		}
	}
}
