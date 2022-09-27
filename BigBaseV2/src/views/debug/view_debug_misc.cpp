#include "fiber_pool.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "thread_pool.hpp"
#include "util/system.hpp"
#include "views/view.hpp"
#include "lua/lua_scripts.hpp"

namespace big
{
	void view::debug_misc() {
		components::button("Load MP Map", []
		{
				DLC::ON_ENTER_MP();
		});
		ImGui::SameLine();
		components::button("Load SP Map", []
		{
			DLC::ON_ENTER_SP();
		});

		components::button("Start Lua", []
			{
				const char* appdata = std::getenv("appdata");
				int r = luaL_dofile(lua_scripts::L, fmt::format("{}/BigBaseV2/lua/autoexec.lua", appdata).c_str());
				if (!r == LUA_OK) {
					const char* errmsg = lua_tostring(lua_scripts::L, -1);
					LOG(WARNING) << errmsg;
				}
			});

		std::string script;
		components::input_text_with_hint("###script", "Lua Script", &script, ImGuiInputTextFlags_EnterReturnsTrue, [script]
			{
				int r = luaL_dostring(lua_scripts::L, script.c_str());
				if (!r == LUA_OK) {
					const char* errmsg = lua_tostring(lua_scripts::L, -1);
					LOG(WARNING) << errmsg;
				}
			});
	}
}