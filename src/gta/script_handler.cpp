/**
 * @file script_handler.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "script_handler.hpp"

#include "pointers.hpp"

#include <network/CNetworkPlayerMgr.hpp>

int CGameScriptHandlerNetComponent::get_participant_index(CNetGamePlayer* player)
{
	if (player == (*big::g_pointers->m_gta.m_network_player_mgr)->m_local_net_player)
		return m_local_participant_index;

	if (m_num_participants <= 1)
		return -1;

	for (int i = 0; i < m_num_participants - 1; i++)
	{
		if (m_participants[i] && m_participants[i]->m_net_game_player == player)
			return m_participants[i]->m_participant_index;
	}

	return -1;
}

bool CGameScriptHandlerNetComponent::is_player_a_participant(CNetGamePlayer* player)
{
	return m_participant_bitset & (1 << player->m_player_id);
}
