#include "lua_globals.hpp"
#include "lua_bindings.hpp"
#include "pointers.hpp"

namespace big::lua::globals
{
	int get_int(int index)
	{
		return *reinterpret_cast<int*>(g_pointers->m_script_globals[index >> 0x12 & 0x3F] + (index & 0x3FFFF));
	}

	void set_int(int index, int value)
	{
		LOG(INFO) << index;
		*reinterpret_cast<int*>(g_pointers->m_script_globals[index >> 0x12 & 0x3F] + (index & 0x3FFFF)) = value;
	}

	float get_float(int index)
	{
		return *reinterpret_cast<float*>(g_pointers->m_script_globals[index >> 0x12 & 0x3F] + (index & 0x3FFFF));
	}

	void set_float(int index, float value)
	{
		*reinterpret_cast<float*>(g_pointers->m_script_globals[index >> 0x12 & 0x3F] + (index & 0x3FFFF)) = value;
	}

	const char* get_string(int index)
	{
		return reinterpret_cast<const char*>(g_pointers->m_script_globals[index >> 0x12 & 0x3F] + (index & 0x3FFFF));
	}

	void load(lua_State* state)
	{
		luabridge::getGlobalNamespace(state)
			.beginNamespace("globals")
				.addFunction("get_int", get_int)
				.addFunction("set_int", set_int)
				.addFunction("get_float", get_float)
				.addFunction("set_float", set_float)
				.addFunction("get_string", get_string)
			.endNamespace();
	}
}