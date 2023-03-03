#include "backend/looped/looped.hpp"
#include "gta/enums.hpp"
#include "natives.hpp"
#include "script.hpp"

namespace big
{
	void looped::vehicle_flares()
	{
		if (!g.vehicle.flares)
			return;

		if (PAD::IS_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_SCRIPTED_FLY_ZUP) && self::veh)
		{
			Hash hash = rage::joaat("weapon_flaregun");
			STREAMING::REQUEST_MODEL(hash);
			WEAPON::REQUEST_WEAPON_ASSET(hash, 31, 26);
			auto offset1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::veh, 0.f, 0.f, -1.0);
			AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "flares_released", self::veh, "DLC_SM_Countermeasures_Sounds", true, 0);
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(self::pos.x,
			    self::pos.y,
			    self::pos.z,
			    offset1.x,
			    offset1.y,
			    offset1.z,
			    0,
			    true,
			    hash,
			    self::ped,
			    true,
			    true,
			    -4.f,
			    self::veh,
			    false,
			    false,
			    false,
			    true,
			    true,
			    false,
			    0);

			script::get_current()->yield(300ms);
		}
	}
}