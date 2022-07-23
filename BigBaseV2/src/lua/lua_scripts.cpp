#include "natives.hpp"
#include "invoker.hpp"
#include "script_global.hpp"
#include "script.hpp"
#include "core/enums.hpp"
#include "file_manager.hpp"
#include "gta/joaat.hpp"
#include "fiber_pool.hpp"
#include "services/notifications/notification_service.hpp"
#include "services/players/player_service.hpp"
#include "lua_scripts.hpp"

#include "util/vehicle.hpp"
#include "util/ped.hpp"
#include "util/notify.hpp"
#include "util/toxic.hpp"
#include "util/teleport.hpp"

#include "native_invoker/lua_native_invoker.hpp"
#include "util/lua_util.hpp"
#include "chat/lua_chat.hpp"
#include "spawn/lua_spawn.hpp"
#include "player/lua_player.hpp"

namespace lua_scripts
{

	static int lua_print(lua_State* L) {
#ifdef PLUTO_VMDUMP
		if (PLUTO_VMDUMP_COND(L)) {
			lua_writestring("<OUTPUT> ", 9);
		}
#endif
		int n = lua_gettop(L);  /* number of arguments */
		int i;
		for (i = 1; i <= n; i++) {  /* for each argument */
			size_t l;
			const char* s = luaL_tolstring(L, i, &l);  /* convert it to string */
			LOG(INFO) << s;  /* print it */
			lua_pop(L, 1);  /* pop result */
		}
		lua_writeline();
		return 0;
	}

	int setLuaPath(lua_State* L, const char* path)
	{
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "path"); // get field "path" from table at top of stack (-1)
		std::string cur_path = lua_tostring(L, -1); // grab path string from top of stack
		cur_path.append(";"); // do your path magic here
		cur_path.append(path);
		lua_pop(L, 1); // get rid of the string on the stack we just pushed on line 5
		lua_pushstring(L, cur_path.c_str()); // push the new one
		lua_setfield(L, -2, "path"); // set the field "path" in table at -2 with value at top of stack
		lua_pop(L, 1); // get rid of package table from top of stack
		return 0; // all done!
	}

	lua_State* L;

	lua_State* init() {
		lua_State* L = luaL_newstate();
		lua_getglobal(L, "_G");
		luaL_openlibs(L);
		lua_register(L, "print", lua_print);
		luaL_requiref(L, "spawn", luaopen_spawn, true);
		luaL_requiref(L, "native_invoker", luaopen_native_invoker, true);
		luaL_requiref(L, "util", luaopen_util, true);
		luaL_requiref(L, "chat", luaopen_chat, true);
		luaL_requiref(L, "player", player::luaopen_player, true);

		const char* appdata = std::getenv("appdata");
		setLuaPath(L, fmt::format("{}/BigBaseV2/lua/?.lua", appdata).c_str());

		lua_pop(L, 1);
		return L;
	}
}