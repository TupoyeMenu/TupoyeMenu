#pragma once

namespace lua_scripts
{
	static int lua_spawn_vehicle(lua_State* L) // Veh spawn(Hash hash, Vector3 location, float heading, bool is_networked = true) // Gay studio shows an error here but compiles fine
	{
		Hash hash = rage::joaat(lua_tostring(L, 1));
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		float z = (float)lua_tonumber(L, 4);
		float heading = (float)lua_tonumber(L, 5);
		bool networked = lua_toboolean(L, 5);

		Vehicle veh = big::vehicle::spawn(hash, Vector3(x, y, z), heading, networked);
		lua_pushnumber(L, veh);

		return 1;
	}

	static int lua_spawn_ped(lua_State* L) // Ped spawn(ePedType pedType, Hash hash, Hash clone, Vector3 location, float heading, bool is_networked = true)
	{
		big::ePedType pedType = (big::ePedType)lua_tointeger(L, 1);
		Hash hash = (Hash)lua_tointeger(L, 2);
		Hash clone = (Hash)lua_tointeger(L, 3);
		float x = (float)lua_tonumber(L, 4);
		float y = (float)lua_tonumber(L, 5);
		float z = (float)lua_tonumber(L, 6);
		float heading = (float)lua_tonumber(L, 7);
		bool networked = lua_toboolean(L, 8);

		Ped ped = big::ped::spawn(pedType, hash, clone, Vector3(x, y, z), heading, networked);
		lua_pushnumber(L, ped);

		return 1;
	}

	int luaopen_spawn(lua_State* L)
	{
		static const struct luaL_Reg lua_spawn[] =
		{
			{"vehicle", lua_spawn_vehicle}, 
			{"ped", lua_spawn_ped}, 
			//{"object", lua_spawn_object},
			{NULL, NULL}
		};

		luaL_newlib(L, lua_spawn);
		return 1;
	}
}
