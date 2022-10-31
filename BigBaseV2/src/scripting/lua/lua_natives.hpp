#pragma once
#include "lua_bindings.hpp"
#include "natives.hpp"

namespace big::lua::natives
{
	void load(sol::state& state);
}