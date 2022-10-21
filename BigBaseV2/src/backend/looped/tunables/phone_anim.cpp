#include "backend/looped/looped.hpp"
#include "script_global.hpp"
#include "natives.hpp"

namespace big
{
	void looped::phone_anim()
	{
		if(g->tunables.phone_anim)
		{
			if (!PED::GET_PED_CONFIG_FLAG(self::ped, 244, false) && !ENTITY::IS_ENTITY_DEAD(self::ped, false))
			{
				PED::SET_PED_CONFIG_FLAG(self::ped, 244, false);
				PED::SET_PED_CONFIG_FLAG(self::ped, 243, false);
				PED::SET_PED_CONFIG_FLAG(self::ped, 242, false);
			}
		}
	}
}