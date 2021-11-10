#include "backend/looped/looped.hpp"
#include "gta/enums.hpp"
#include "natives.hpp"
#include "util/entity.hpp"

namespace big
{
	//menyoo horn boost
	void looped::vehicle_horn_boost()
	{
		if (!g.vehicle.horn_boost) return;

		if (PAD::IS_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_HORN))
		{
			Ped ped = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
			if (veh == 0) return;

			entity::take_control_of(veh);

			AUDIO::SET_VEHICLE_BOOST_ACTIVE(veh, true);
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 1.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			AUDIO::SET_VEHICLE_BOOST_ACTIVE(veh, false);

			
		}
	}
}

/*	void looped::vehicle_horn_boost()
	{
		if (!g.vehicle.horn_boost) return;

		if (PAD::IS_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_HORN))
		{
			Ped ped = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);

			if (veh == 0) return;
			if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, 0) != ped) return;

			Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(veh) * 1.05f;
			ENTITY::SET_ENTITY_VELOCITY(veh, velocity.x, velocity.y, velocity.z - 0.5f);
		}
	}
}*/