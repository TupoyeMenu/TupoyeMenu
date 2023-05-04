/**
 * @file increment_stat_event.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta/net_game_event.hpp"
#include "hooking.hpp"
#include "services/players/player_service.hpp"

#include <network/CNetGamePlayer.hpp>

namespace big
{
	bool hooks::increment_stat_event(CNetworkIncrementStatEvent* net_event, CNetGamePlayer* sender)
	{
		switch (net_event->m_stat)
		{
		case RAGE_JOAAT("MPPLY_BAD_CREW_STATUS"):
		case RAGE_JOAAT("MPPLY_BAD_CREW_MOTTO"):
		case RAGE_JOAAT("MPPLY_BAD_CREW_NAME"):
		case RAGE_JOAAT("MPPLY_BAD_CREW_EMBLEM"):
		case RAGE_JOAAT("MPPLY_EXPLOITS"):
		case RAGE_JOAAT("MPPLY_GAME_EXPLOITS"):
		case RAGE_JOAAT("MPPLY_TC_ANNOYINGME"):
		case RAGE_JOAAT("MPPLY_TC_HATE"):
		case RAGE_JOAAT("MPPLY_VC_ANNOYINGME"):
		case RAGE_JOAAT("MPPLY_VC_HATE"):
			g.reactions.report.process(g_player_service->get_by_id(sender->m_player_id));
			return true;
		}

		if(g.reactions.report.log)
			LOG(VERBOSE) << std::format("Recieved Remote Stat: {} | Sender: {}", net_event->m_stat, sender->get_name());

		return false;
	}
}