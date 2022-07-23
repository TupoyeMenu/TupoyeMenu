#include "hooking.hpp"
#include "services/players/player_service.hpp"
#include "lua/lua_scripts.hpp"

namespace big
{
	void hooks::player_leave(CNetworkObjectMgr* _this, CNetGamePlayer* net_player)
	{
		g_player_service->player_leave(net_player);

		if (const rage::netPlayerData* net_player_data = net_player->get_net_data(); net_player_data)
		{
			if (g->notifications.player_leave.log)
				LOG(INFO) << "Player left, " << net_player_data->m_name << ", " //name
				<< (int)net_player->m_player_id << ", " //id
				<< net_player_data->m_rockstar_id2; //rid

			if (g->notifications.player_leave.notify)
				g_notification_service->push("Player Left", fmt::format("{} freeing slot #{} with Rockstar ID: {}", net_player_data->m_name, net_player->m_player_id, net_player_data->m_rockstar_id2));

			lua_getglobal(lua_scripts::L, "hook");
			lua_getfield(lua_scripts::L, -1, "Call");
			lua_pushstring(lua_scripts::L, "on_player_left");
			lua_pushstring(lua_scripts::L, net_player_data->m_name);
			lua_pushinteger(lua_scripts::L, net_player->m_player_id);
			lua_pushinteger(lua_scripts::L, net_player_data->m_rockstar_id2);
			if (lua_pcall(lua_scripts::L, 4, 0, 0))
			{
				LOG(INFO) << lua_tostring(lua_scripts::L, -1);
				lua_pop(lua_scripts::L, 1);
			}
		}

		return g_hooking->m_player_has_left_hook.get_original<decltype(&hooks::player_leave)>()(_this, net_player);
	}
}