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
					g_lua_manager->draw_gui("Threads"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Script Events"))
			{
				view::debug_script_events();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Script Events"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Globals"))
			{
				view::debug_globals();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Globals"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Locals"))
			{
				view::debug_locals();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Locals"_J);
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
					g_lua_manager->draw_gui("Misc"_J);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
