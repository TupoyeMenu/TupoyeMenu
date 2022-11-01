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

	void load(sol::state& state)
	{
		auto ns = state["globals"].get_or_create<sol::table>();
		ns["get_int"] = get_int;
		ns["set_int"] = set_int;
		ns["get_float"] = get_float;
		ns["set_float"] = set_float;
		ns["get_string"] = get_string;
	}
}