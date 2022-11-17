#pragma once

namespace lua_scripts
{
	static int lua_begin_call(lua_State* L)
	{
		big::g_native_invoker.begin_call();

		return 0;
	}

	static int lua_end_call(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			std::string address_str = lua_tostring(L, 1);

			std::uint64_t address_hash;

			std::istringstream stream(address_str);

			stream >> std::hex >> address_hash;

			big::g_native_invoker.end_call(address_hash);
		}
		return 0;
	}

	static int lua_push_arg_int(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			int int_arg = (int)lua_tointeger(L, 1);
			big::g_native_invoker.push_arg(int_arg);
		}
		return 0;
	}

	static int lua_push_arg_float(lua_State* L)
	{
		if (lua_isnumber(L, 1))
		{
			float float_arg = (float)lua_tonumber(L, 1);
			big::g_native_invoker.push_arg(float_arg);
		}
		return 0;
	}

	static int lua_push_arg_bool(lua_State* L)
	{
		if (lua_isboolean(L, 1))
		{
			bool bool_arg = lua_toboolean(L, 1);
			big::g_native_invoker.push_arg(bool_arg);
		}
		return 0;

	}

	static int lua_push_arg_string(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			const char* string_arg = lua_tostring(L, 1);
			big::g_native_invoker.push_arg(string_arg);
		}
		return 0;
	}

	static int lua_push_arg_pointer(lua_State* L)
	{
		if (lua_isinteger(L, 1))
		{
			int pointer_arg = (int)lua_tointeger(L, 1);
			big::g_native_invoker.push_arg(&pointer_arg);
		}
		return 0;
	}


	static int lua_get_return_value_int(lua_State* L)
	{
		lua_pushinteger(L, big::g_native_invoker.get_return_value<int>());
		return 1;
	}

	static int lua_get_return_value_float(lua_State* L)
	{
		lua_pushnumber(L, big::g_native_invoker.get_return_value<float>());
		return 1;
	}

	static int lua_get_return_value_bool(lua_State* L)
	{
		lua_pushboolean(L, big::g_native_invoker.get_return_value<bool>());
		return 1;
	}

	static int lua_get_return_value_string(lua_State* L)
	{
		lua_pushstring(L, big::g_native_invoker.get_return_value<const char*>());
		return 1;
	}

	static int lua_get_return_value_vector3(lua_State* L)
	{
		Vector3 out = big::g_native_invoker.get_return_value<Vector3>();

		lua_createtable(L, 0, 3);

		lua_pushstring(L, "x"); // is very stupid
		lua_pushnumber(L, out.x);
		lua_settable(L, -3);

		lua_pushstring(L, "y");
		lua_pushnumber(L, out.y);
		lua_settable(L, -3);

		lua_pushstring(L, "z");
		lua_pushnumber(L, out.z);
		lua_settable(L, -3);

		return 1;
	}

	int luaopen_native_invoker(lua_State* L)
	{
		static const struct luaL_Reg native_invoker[] =
		{
			{"begin_call", lua_begin_call}, // native_invoker.begin_call()
			{"end_call", lua_end_call}, // native_invoker.end_call(char hash)

			{"push_arg_int", lua_push_arg_int}, // native_invoker.push_arg_int(int)
			{"push_arg_float", lua_push_arg_float}, // native_invoker.push_arg_float(float)
			{"push_arg_bool", lua_push_arg_bool}, // native_invoker.push_arg_bool(bool)
			{"push_arg_string", lua_push_arg_string}, // native_invoker.push_arg_string(char)
			{"push_arg_pointer", lua_push_arg_pointer}, // native_invoker.push_arg_pointer(ptr)

			{"get_return_value_int", lua_get_return_value_int}, // native_invoker.get_return_value_int()
			{"get_return_value_float", lua_get_return_value_float}, // native_invoker.get_return_value_float()
			{"get_return_value_bool", lua_get_return_value_bool}, // native_invoker.get_return_value_bool()
			{"get_return_value_string", lua_get_return_value_string}, // native_invoker.get_return_value_string()
			{"get_return_value_vector3", lua_get_return_value_vector3}, // native_invoker.get_return_value_vector3()

			{NULL, NULL}
		};

		luaL_newlib(L, native_invoker);
		return 1;
	}
}