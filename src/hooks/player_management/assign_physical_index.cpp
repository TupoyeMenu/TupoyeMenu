/**
 * @file assign_physical_index.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "backend/player_command.hpp"
#include "core/data/admin_rids.hpp"
#include "core/globals.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "hooking.hpp"
#include "lua/lua_manager.hpp"
#include "packet.hpp"
#include "services/player_database/player_database_service.hpp"
#include "services/players/player_service.hpp"
#include "util/notify.hpp"
#include "util/session.hpp"

#include <network/Network.hpp>


namespace big
{

	void* hooks::assign_physical_index(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, uint8_t new_index)
	{
		const auto* net_player_data = player->get_net_data();

		if (new_index == static_cast<uint8_t>(-1))
		{
			g.m_spoofed_peer_ids.erase(player->get_net_data()->m_host_token);
			g_player_service->player_leave(player);

			if (net_player_data)
			{
				g_lua_manager->trigger_event<"player_leave">(net_player_data->m_name);

				if (g.notifications.player_leave.log)
					LOG(INFO) << "Player left '" << net_player_data->m_name << "' freeing slot #" << (int)player->m_player_id
					          << " with Rockstar ID: " << net_player_data->m_gamer_handle.m_rockstar_id;

				if (g.notifications.player_leave.notify)
				{
					g_notification_service->push("Player Left",
					    std::format("{} freeing slot {} with Rockstar ID: {}",
					        net_player_data->m_name,
					            player->m_player_id,
					            net_player_data->m_gamer_handle.m_rockstar_id));
				}
			}

			return g_hooking->get_original<hooks::assign_physical_index>()(netPlayerMgr, player, new_index);
		}

		const auto result = g_hooking->get_original<hooks::assign_physical_index>()(netPlayerMgr, player, new_index);
		g_player_service->player_join(player);
		if (net_player_data)
		{
			if (g.protections.admin_check)
			{
				if (admin_rids.contains(net_player_data->m_gamer_handle.m_rockstar_id))
				{
					g_notification_service->push_warning("Potential Admin Found!",
					    std::vformat("{} has been detected as admin", std::make_format_args(net_player_data->m_name)));

					LOG(WARNING) << net_player_data->m_name << " (" << net_player_data->m_gamer_handle.m_rockstar_id << ") has been detected as an admin";

					auto id = player->m_player_id;
					if (auto plyr = g_player_service->get_by_id(id))
						plyr->is_rockstar_admin = true;
				}
			}

			g_lua_manager->trigger_event<"player_join">(net_player_data->m_name, player->m_player_id);

			if (g.notifications.player_join.above_map && *g_pointers->m_gta.m_is_session_started) // prevent loading screen spam
				notify::player_joined(player);

			if (g.notifications.player_join.log)
				LOG(INFO) << "Player joined '" << net_player_data->m_name << "' allocating slot #" << (int)player->m_player_id
				          << " with Rockstar ID: " << net_player_data->m_gamer_handle.m_rockstar_id;

			if (g.notifications.player_join.notify)
			{
				g_notification_service->push("Player Joined",
				    std::format("{} taking slot {} with Rockstar ID: {}",
				        net_player_data->m_name,
				            player->m_player_id,
				            net_player_data->m_gamer_handle.m_rockstar_id));
			}

			auto id           = player->m_player_id;
			bool lock_session = g.session.lock_session;

			g_fiber_pool->queue_job([id, lock_session] {
				if (auto plyr = g_player_service->get_by_id(id))
				{
					if (plyr->get_net_data()->m_gamer_handle.m_rockstar_id != 0)
					{
						if (auto entry = g_player_database_service->get_player_by_rockstar_id(
						        plyr->get_net_data()->m_gamer_handle.m_rockstar_id))
						{
							plyr->is_modder         = entry->is_modder;
							plyr->block_join        = entry->block_join;
							plyr->block_join_reason = entry->block_join_reason;

							if (strcmp(plyr->get_name(), entry->name.data()))
							{
								g_notification_service->push("Players",
								    std::format("{} changed their name to {}", entry->name, plyr->get_name()));
								entry->name = plyr->get_name();
								g_player_database_service->save();
							}
						}
					}
					if (plyr->block_join)
					{
						dynamic_cast<player_command*>(command::get(RAGE_JOAAT("breakup")))->call(plyr, {});
						g_notification_service->push_warning("Block Join",
						    std::format("Block Join method failed for {}, sending breakup kick instead...",
						        plyr->get_net_data()->m_name));
						LOG(WARNING) << "Sending Breakup Kick due to block join failure... ";
					}
					if (lock_session)
					{
						dynamic_cast<player_command*>(command::get(RAGE_JOAAT("breakup")))->call(plyr, {});
						g_notification_service->push_warning("Lock Session",
						    std::format("A player with the name of {} has been denied entry", plyr->get_net_data()->m_name));
					}
				}
			});
		}
		return result;
	}

}
