#include "backend/looped/looped.hpp"
#include "pointers.hpp"
#include "natives.hpp"

namespace big
{

	void looped::player_freeze()
	{
		if (!g.selected_player.is_online || !g.player.freezing)
		{
			return;
		}

		Ped target = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g.selected_player.id);

		TASK::CLEAR_PED_TASKS_IMMEDIATELY(target);
	}
}