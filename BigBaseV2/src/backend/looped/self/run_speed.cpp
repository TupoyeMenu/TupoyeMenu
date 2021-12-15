#include "backend/looped/looped.hpp"

namespace big
{
	static float iLastrunspeed = 1.f;

	void looped::self_run_speed()
	{
		if (g_local_player == nullptr || g_local_player->m_player_info == nullptr) return;

		if (g.self.run_speed != iLastrunspeed)
			g_local_player->m_player_info->m_run_speed = g.self.run_speed;
	}
}