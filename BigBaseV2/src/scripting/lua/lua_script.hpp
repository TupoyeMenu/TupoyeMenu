#pragma once
#include "script.hpp"
#include "lua_bindings.hpp"

namespace big
{
	namespace lua::script
	{
		void load(lua_State* state);
	}

	class lua_script
	{
	public:
		lua_State* m_state;
		std::optional<int> m_script_tick_function;
		std::vector<script*> m_scripts;

		lua_script(std::filesystem::path path);
		~lua_script();
	};
}


namespace luabridge
{
	template <>
	struct Stack <big::lua_script*>
	{
		static void push(lua_State* L, big::lua_script* s)
		{
			lua_pushlightuserdata(L, s);
		}

		static big::lua_script* get(lua_State* L, int index)
		{
			return (big::lua_script*)lua_touserdata(L, index);
		}

		static bool isInstance(lua_State* L, int index)
		{
			return lua_type(L, index) == LUA_TLIGHTUSERDATA;
		}
	};
}