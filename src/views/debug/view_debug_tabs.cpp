#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::debug_tabs()
	{
		if (ImGui::BeginTabBar("debug_tabbar"))
		{
			if (ImGui::BeginTabItem("Threads"))
			{
				view::debug_threads();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Threads"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG_TAB_SCRIPT_EVENTS"_T.data()))
			{
				view::debug_script_events();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_SCRIPT_EVENTS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG_TAB_GLOBALS"_T.data()))
			{
				view::debug_globals();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_GLOBALS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG_TAB_LOCALS"_T.data()))
			{
				view::debug_locals();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_LOCALS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Ped Flags"))
			{
				view::debug_ped_flags();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Ped Flags"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Animations"))
			{
				view::debug_animations();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Animations"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG_TAB_MISC"_T.data()))
			{
				view::debug_misc();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_MISC"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
