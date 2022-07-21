#pragma once

namespace lua_scripts
{
	static int lua_joaat(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			Hash hash = rage::joaat(lua_tostring(L, 1));
			lua_pushinteger(L, hash);
			return 1;
		}
		return 0;
	}

	static int _lua_yield(lua_State* L)
	{
		int n = lua_gettop(L);  /* number of arguments */
		if (n && lua_isinteger(L, 1))
		{
			big::script::get_current()->yield(std::chrono::milliseconds(lua_tointeger(L, 1)));
		}
		else
		{
			big::script::get_current()->yield();
		}
		return 0;
	}

	static int lua_notify_top(lua_State* L)
	{
		if (lua_isstring(L, 1) && lua_isstring(L, 2))
		{
			big::g_notification_service->push(lua_tostring(L, 1), lua_tostring(L, 2));
		}
		return 0;
	}

	static int lua_notify_map(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			big::notify::above_map(lua_tostring(L, 1));
		}
		return 0;
	}

	int luaopen_util(lua_State* L)
	{
		static const struct luaL_Reg util[] =
		{
			{"yield", _lua_yield}, // util.yield(?int ms)
			{"joaat", lua_joaat}, // util.joaat(char hash)
			{"notify_top", lua_notify_top}, // util.notify_top(char text, char text2)
			{"notify_map", lua_notify_map}, // util.notify_map(char text)
			{NULL, NULL}
		};

		luaL_newlib(L, util);
		return 1;
	}
}