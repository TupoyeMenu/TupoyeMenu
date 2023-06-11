/**
 * @file view_debug_tabs.cpp
 * @brief Debug options tab bar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
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
					g_lua_manager->draw_gui(RAGE_JOAAT("Threads"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Script Events"))
			{
				view::debug_script_events();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_SCRIPT_EVENTS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Globals"))
			{
				view::debug_globals();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_GLOBALS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Locals"))
			{
				view::debug_locals();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("DEBUG_TAB_LOCALS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Animations"))
			{
				view::debug_animations();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Animations"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
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
