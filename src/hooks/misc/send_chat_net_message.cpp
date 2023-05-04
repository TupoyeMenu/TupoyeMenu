/**
 * @file send_chat_net_message.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta_util.hpp"
#include "hooking.hpp"
#include "services/players/player_service.hpp"

#include <network/CMsgTextMessage.hpp>
#include <network/Network.hpp>

namespace big
{
	bool hooks::send_chat_net_message(rage::netConnectionManager* mgr, int receiver_msg_id, CMsgTextMessage* msg, int flags, void* unk)
	{
		if (g.m_spoofed_peer_ids.contains(msg->m_peer_id))
			msg->m_peer_id = g.m_spoofed_peer_ids[msg->m_peer_id];

		return g_hooking->get_original<hooks::send_chat_net_message>()(mgr, receiver_msg_id, msg, flags, unk);
	}
}