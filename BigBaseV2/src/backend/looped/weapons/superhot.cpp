#include "backend/looped/looped.hpp"
#include "core/enums.hpp"
#include "util/entity.hpp"
#include "util/math.hpp"
#include "util/notify.hpp"

namespace big
{
	//that's stupid
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
	/*
	from https://github.com/gta-chaos-mod/ChaosModV
	Effect by Last0xygen
	*/

	static int lastCheck = 0;

	void looped::weapons_superhot()
	{
		if (g.weapons.superhot)
		{
			int current_time = MISC::GET_GAME_TIMER();
			if (current_time - lastCheck > 100)
			{
				lastCheck = current_time;
				Ped playerPed = PLAYER::PLAYER_PED_ID();
				float gameSpeed = 1;
				if (!PED::IS_PED_GETTING_INTO_A_VEHICLE(playerPed) && !PED::IS_PED_CLIMBING(playerPed) && !PED::IS_PED_DIVING(playerPed) && !PED::IS_PED_JUMPING_OUT_OF_VEHICLE(playerPed) && !PED::IS_PED_RAGDOLL(playerPed) && !TASK::IS_PED_GETTING_UP(playerPed))
				{
					float speed = ENTITY::GET_ENTITY_SPEED(playerPed);
					gameSpeed = max(min(speed, 4.f) / 4, 0.2);
				}
				MISC::SET_TIME_SCALE(gameSpeed);
			}
		}
	}
}