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
			const char* text = "";

			//ImGui::InputTextMultiline("##code", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20));

			static TextEditor editor;

			editor.SetShowWhitespaces(false);
			editor.SetReadOnly(false);
			editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

			

			editor.Render("##code", ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20));
			if (editor.IsHandleKeyboardInputsEnabled())
				g_fiber_pool->queue_job([] {
					PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
				});

			text = editor.GetText().c_str();

			components::button("Start", [text]
			{
				lua_State* L = luaL_newstate();
				lua_scripts::init(L);
				int r = luaL_dostring(L, text);
				if (!r == LUA_OK) {
					const char* errmsg = lua_tostring(L, -1);
					LOG(WARNING) << errmsg;
				}

				lua_pop(L, 1);
				lua_close(L);
			});

		}
	}
}