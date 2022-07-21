#include "natives.hpp"
#include "invoker.hpp"
#include "script_global.hpp"
#include "script.hpp"
#include "core/enums.hpp"
#include "file_manager.hpp"
#include "lua_scripts.hpp"
#include "gta/joaat.hpp"
#include "fiber_pool.hpp"
#include "services/notifications/notification_service.hpp"

#include "util/vehicle.hpp"
#include "util/ped.hpp"
#include "util/notify.hpp"

#include "native_invoker/lua_native_invoker.hpp"
#include "util/lua_util.hpp"
#include "chat/lua_chat.hpp"
#include "spawn/lua_spawn.hpp"

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

	void init(lua_State *L) {
		lua_getglobal(L, "_G");
		luaL_openlibs(L);
		lua_register(L, "print", lua_print);
		luaL_requiref(L, "spawn", luaopen_spawn, true);
		luaL_requiref(L, "native_invoker", luaopen_native_invoker, true);
		luaL_requiref(L, "util", luaopen_util, true);
		luaL_requiref(L, "chat", luaopen_chat, true);

		const char* base_dir = std::getenv("appdata");

		int r = luaL_dofile(L, fmt::format("{}/BigBaseV2/lua/autoexec.lua", base_dir).c_str());
		if (!r == LUA_OK) {
			const char* errmsg = lua_tostring(L, -1);
			LOG(WARNING) << errmsg;
		}
	}
}