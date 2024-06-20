/**
 * @file friends_service.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "friends_service.hpp"

#include "pointers.hpp"

#include <network/CNetGamePlayer.hpp>
#include <socialclub/FriendRegistry.hpp>

namespace big
{
	friends_service::friends_service()
	{
		g_friends_service = this;
	}

	friends_service::~friends_service()
	{
		g_friends_service = nullptr;
	}

	bool friends_service::is_friend(int64_t rockstar_id)
	{
		for (uint32_t i = 0; i < g_pointers->m_gta.m_friend_registry->m_friend_count; i++)
			if (rockstar_id == g_pointers->m_gta.m_friend_registry->get(i)->m_rockstar_id)
				return true;

		return false;
	}

	bool friends_service::is_friend(CNetGamePlayer* net_player)
	{
		if (net_player == nullptr)
			return false;

		if (auto net_data = net_player->get_net_data())
		{
			const auto rockstar_id = net_data->m_gamer_handle.m_rockstar_id;
			return is_friend(rockstar_id);
		}
		return false;
	}

}