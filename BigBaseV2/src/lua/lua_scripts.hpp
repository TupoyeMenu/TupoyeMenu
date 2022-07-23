#pragma once
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

namespace lua_scripts
{
	extern lua_State* L;
	lua_State* init();
}