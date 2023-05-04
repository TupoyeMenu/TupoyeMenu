/**
 * @file send_player_card_stats.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "network/Network.hpp"

#include <stats/CPlayerCardStats.hpp>

namespace big
{
	rage::netGameEvent* hooks::send_player_card_stats(rage::netGameEvent* a1, CPlayerCardStats* stats)
	{
		if (g.spoofing.spoof_rank)
			stats->m_rank = g.spoofing.rank;

		if (g.spoofing.spoof_kd_ratio)
			stats->m_kd_ratio = g.spoofing.kd_ratio;

		if (g.spoofing.spoof_bad_sport)
		{
			switch (g.spoofing.badsport_type)
			{
			case 0: stats->m_overall_badsport = 0.0f; break;
			case 1: stats->m_overall_badsport = 46.0f; break;
			case 2: stats->m_overall_badsport = 1000.0f; break;
			}
		}

		return g_hooking->get_original<hooks::send_player_card_stats>()(a1, stats);
	}
}