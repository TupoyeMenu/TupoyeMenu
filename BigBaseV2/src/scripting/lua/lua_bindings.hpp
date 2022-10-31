#pragma once

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <luaconf.h>
#include <lauxlib.h>
#include <lstate.h>
#include <lapi.h>
}

#undef cast
#include "sol.hpp"