/**
 * @file network_player_mgr.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/menu_event.hpp"
#include "hooking.hpp"
#include "lua/lua_manager.hpp"
#include "pointers.hpp"
#include "services/players/player_service.hpp"

#include <network/CNetworkPlayerMgr.hpp>

namespace big
{
	bool hooks::network_player_mgr_init(CNetworkPlayerMgr* _this, uint64_t a2, uint32_t a3, uint32_t a4[4])
	{
		if (g.notifications.network_player_mgr_init.log)
			LOG(INFO) << "CNetworkPlayerMgr#init got called, we're probably entering a session.";
		if (g.notifications.network_player_mgr_init.notify)
			g_notification_service->push("Network Player Manager", "Entering session and initializing player data.");

		bool result = g_hooking->get_original<hooks::network_player_mgr_init>()(_this, a2, a3, a4);

		g_player_service->player_join(_this->m_local_net_player);
		g_lua_manager->trigger_event<menu_event::PlayerMgrInit>();

		return result;
	}

	void hooks::network_player_mgr_shutdown(CNetworkPlayerMgr* _this)
	{
		g_player_service->do_cleanup();

		if (g.notifications.network_player_mgr_shutdown.log)
			LOG(INFO) << "CNetworkPlayerMgr#shutdown got called, we're probably leaving our session.";
		if (g.notifications.network_player_mgr_shutdown.notify)
			g_notification_service->push("Network Player Manager", "Leaving session and cleaning up player data.");

		g_hooking->get_original<hooks::network_player_mgr_shutdown>()(_this);
		g_lua_manager->trigger_event<menu_event::PlayerMgrShutdown>();
	}
}
