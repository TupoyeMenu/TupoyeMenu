#include "views/view.hpp"
#include "natives.hpp"
#include "fiber_pool.hpp"
#include "hooks/chat/chat.hpp"
#include "pointers.hpp"
#include "services/players/player_service.hpp"
#include "lua/lua_scripts.hpp"
#include "widgets/imgui_texteditor.h"

namespace big
{
	void view::lua_editor()
    {
		if (ImGui::Begin("Lua editor"))
		{

			//ImGui::InputTextMultiline("##code", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20));
			/*
			static TextEditor editor;

			editor.SetShowWhitespaces(false);
			editor.SetReadOnly(false);
			editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

			

			editor.Render("##code", ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20));
			if (editor.IsHandleKeyboardInputsEnabled())
				g_fiber_pool->queue_job([] {
					PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
				});

			text = editor.GetText().c_str();*/

			components::button("test", []
			{
				lua_getglobal(lua_scripts::L, "hook");
				lua_getfield(lua_scripts::L, -1, "Call");
				lua_pushstring(lua_scripts::L, "on_player_left");
				lua_pushstring(lua_scripts::L, "name");
				lua_pushinteger(lua_scripts::L, 8);
				lua_pushinteger(lua_scripts::L, 83);
				if (lua_pcall(lua_scripts::L, 4, 0, 0))
				{
					LOG(INFO) << lua_tostring(lua_scripts::L, -1);
					lua_pop(lua_scripts::L, 1);
				}
			});

			components::button("Start", []
			{
				const char* appdata = std::getenv("appdata");
				int r = luaL_dofile(lua_scripts::L, fmt::format("{}/BigBaseV2/lua/autoexec.lua", appdata).c_str());
				if (!r == LUA_OK) {
					const char* errmsg = lua_tostring(lua_scripts::L, -1);
					LOG(WARNING) << errmsg;
				}
			});

		}
	}
}