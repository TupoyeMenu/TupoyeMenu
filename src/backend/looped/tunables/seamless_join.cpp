#include "backend/looped/looped.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script_global.hpp"
#include "script.hpp"
#include "core/enums.hpp"
#include "core/scr_globals.hpp"

namespace big
{
	void looped::seamless_join()
	{
		if (g.tunables.seamless_join)
		{
			const auto state = *scr_globals::transition_state.as<eTransitionState*>();

			if (state <= eTransitionState::TRANSITION_STATE_FM_FINAL_SETUP_PLAYER)
			{
				PLAYER::SET_PLAYER_CONTROL(self::id, true, 0);
				if (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
				{
					STREAMING::STOP_PLAYER_SWITCH();
					GRAPHICS::ANIMPOSTFX_STOP_ALL(); // fix Skycam effect
				}
			}
		}
	}
}