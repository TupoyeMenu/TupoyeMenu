/**
 * @file view_debug_tabs.cpp
 * @brief Debug options tab bar.
 */

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
					g_lua_manager->draw_gui("VIEW_DEBUG_THREADS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Script Events"))
			{
				view::debug_script_events();
				if (g_lua_manager)
					g_lua_manager->draw_gui("DEBUG_TAB_SCRIPT_EVENTS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Globals"))
			{
				view::debug_globals();
				if (g_lua_manager)
					g_lua_manager->draw_gui("DEBUG_TAB_GLOBALS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Locals"))
			{
				view::debug_locals();
				if (g_lua_manager)
					g_lua_manager->draw_gui("DEBUG_TAB_LOCALS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Animations"))
			{
				view::debug_animations();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Animations"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				view::debug_misc();
				if (g_lua_manager)
					g_lua_manager->draw_gui("DEBUG_TAB_MISC"_J);
				ImGui::EndTabItem();
			}

			if (g_lua_manager)
				g_lua_manager->draw_child_tabs_external("GUI_TAB_DEBUG"_J);

			ImGui::EndTabBar();
		}
	}
}
