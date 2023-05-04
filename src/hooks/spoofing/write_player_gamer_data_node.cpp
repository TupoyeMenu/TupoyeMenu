/**
 * @file write_player_gamer_data_node.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"

#include <netsync/nodes/player/CPlayerGamerDataNode.hpp>

namespace big
{
	void hooks::write_player_gamer_data_node(rage::netObject* player, CPlayerGamerDataNode* node)
	{
		g_hooking->get_original<write_player_gamer_data_node>()(player, node);

		if (g.spoofing.spoof_crew_data)
		{
			node->m_clan_data.m_clan_member_id    = 1;
			node->m_clan_data.m_clan_id           = 41564112;
			node->m_clan_data.m_clan_id_2         = 41564112;
			node->m_clan_data.m_clan_color        = 420;
			node->m_clan_data.m_clan_member_count = 15;
			node->m_clan_data.m_clan_created_time = 420;

			strcpy(node->m_clan_data.m_clan_tag, g.spoofing.crew_tag.c_str());
			node->m_clan_data.m_is_system_clan = g.spoofing.rockstar_crew;
			node->m_clan_data.m_is_clan_open   = g.spoofing.square_crew_tag;

			if (g.spoofing.rockstar_crew)
			{
				strcpy(node->m_clan_data.m_clan_name, "Rockstar");
			}
		}

		node->m_is_cheater = g.spoofing.spoof_cheater;

		// spoof r* dev and qa only when spoofing rid because every player in the session will send metrics if you join with that enabled
		/*if (g.spoofing.spoof_rockstar_id)
		{
			node->m_is_rockstar_dev = g.spoofing.spoof_rockstar_dev;
			node->m_is_rockstar_qa = g.spoofing.spoof_rockstar_qa;
		}*/
	}
}
