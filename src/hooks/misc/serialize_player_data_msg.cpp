/**
 * @file serialize_player_data_msg.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta_util.hpp"
#include "hooking.hpp"

#include <network/CNetGamePlayerDataMsg.hpp>
#include <network/Network.hpp>

namespace big
{
	bool hooks::serialize_player_data_msg(CNetGamePlayerDataMsg* msg, rage::datBitBuffer* buffer)
	{
		int old_group = msg->m_matchmaking_group;

		if (g.session.join_in_sctv_slots)
			msg->m_matchmaking_group = 4;

		bool ret                 = g_hooking->get_original<hooks::serialize_player_data_msg>()(msg, buffer);
		msg->m_matchmaking_group = old_group;
		return ret;
	}
}