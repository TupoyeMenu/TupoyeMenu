#pragma once


using namespace big;
namespace lua_scripts::player
{

	static int lua_teleport(lua_State* L)
	{
		if (lua_isinteger(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4))
		{
			lua_pushboolean(L, teleport::teleport_player((int)lua_tointeger(L, 1), Vector3((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4))));
		}
		return 1;
	}

	static int lua_taze(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			toxic::taze_player((int)lua_tointeger(L, 1));
		}
		return 0;
	}

	static int lua_bounty(lua_State* L)
	{
		if (lua_isinteger(L, 1) && lua_isinteger(L, 2))
		{
			toxic::bounty_player((int)lua_tointeger(L, 1), (int)lua_tointeger(L, 2));
		}
		return 0;
	}

	static int lua_desync_kick(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			CNetGamePlayer* target = g_player_service->get_by_player_id((int)lua_tointeger(L, 1))->get_net_game_player();
			toxic::desync_kick(target);
		}
		return 0;
	}

	static int lua_is_host(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			bool host = g_player_service->get_by_player_id((int)lua_tointeger(L, 1))->is_host();
			lua_pushboolean(L, host);
		}
		return 1;
	}

	static int lua_is_friend(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			bool _friend = g_player_service->get_by_player_id((int)lua_tointeger(L, 1))->is_friend();
			lua_pushboolean(L, _friend);
		}
		return 1;
	}
	
	static int lua_get_rid(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			uint64_t rid = g_player_service->get_by_player_id((int)lua_tointeger(L, 1))->get_net_data()->m_rockstar_id2;
			lua_pushinteger(L, rid);
		}
		return 1;
	}

	static int lua_get_ip(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			rage::netPlayerData* player = g_player_service->get_by_player_id((int)lua_tointeger(L, 1))->get_net_data();
			lua_pushinteger(L, player->m_external_ip.m_field1);
			lua_pushinteger(L, player->m_external_ip.m_field2);
			lua_pushinteger(L, player->m_external_ip.m_field3);
			lua_pushinteger(L, player->m_external_ip.m_field4);
		}
		return 4;
	}

	int luaopen_player(lua_State* L)
	{
		static const struct luaL_Reg player[] =
		{
			{"teleport", lua_teleport },
			{"taze", lua_taze },
			{"bounty", lua_bounty },
			{"desync_kick", lua_desync_kick },
			{"is_host", lua_is_host },
			{"is_friend", lua_is_friend },
			{"get_rid", lua_get_rid },
			{"get_ip", lua_get_ip },
			{NULL, NULL}
		};

		luaL_newlib(L, player);
		return 1;
	}
}
