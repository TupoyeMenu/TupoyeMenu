/**
 * @file view_self_tabs.cpp
 * @brief Self options tab bar.
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
	void view::self_tabs()
	{
		if (ImGui::BeginTabBar("self_tabbar"))
		{
			if (ImGui::BeginTabItem("Self"))
			{
				view::self();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SELF"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Weapons"))
			{
				view::weapons();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_WEAPONS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Mobile"))
			{
				view::mobile();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_MOBILE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Teleport"))
			{
				view::teleport();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_TELEPORT"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Custom Teleport"))
			{
				view::custom_teleport();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_CUSTOM_TELEPORT"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Outfit Editor"))
			{
				view::outfit_editor();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_OUTFIT_EDITOR"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Outfit Slots"))
			{
				view::outfit_slots();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_OUTFIT_SLOTS"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
