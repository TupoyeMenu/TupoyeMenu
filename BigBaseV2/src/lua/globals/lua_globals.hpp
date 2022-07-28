#pragma once

using namespace big;
namespace lua_scripts::globals
{
	static int lua_get_global_int(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			int global = *script_global(lua_tointeger(L, 1)).as<int*>();
			lua_pushinteger(L, global);
		}
		return 1;
	}

	static int lua_get_global_float(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			float global = *script_global(lua_tointeger(L, 1)).as<float*>();
			lua_pushnumber(L, global);
		}
		return 1;
	}

	static int lua_get_global_string(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			const char global = *script_global(lua_tointeger(L, 1)).as<char*>();
			lua_pushnumber(L, global);
		}
		return 1;
	}

	static int lua_set_global_number(lua_State* L)
	{
		if (lua_isinteger(L, 1) && lua_isnumber(L, 2))
		{
			if (lua_isinteger(L, 2))
			{
				*script_global((int)lua_tointeger(L, 1)).as<int*>() = (int)lua_tointeger(L, 2);
			}
			else
			{
				*script_global((int)lua_tointeger(L, 1)).as<float*>() = (float)lua_tonumber(L, 2);
			}
		}
		return 0;
	}

	int luaopen_globals(lua_State* L)
	{
		static const struct luaL_Reg globals[] =
		{
			{"get_global_int", lua_get_global_int},
			{"get_global_float", lua_get_global_float},
			{"get_global_string", lua_get_global_string},

			{"set_global_number", lua_set_global_number},

			{NULL, NULL}
		};

		luaL_newlib(L, globals);
		return 1;
	}
}