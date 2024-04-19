/**
 * @file view_network_tabs.cpp
 * @brief Network options tab bar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::network_tabs()
	{
		if (ImGui::BeginTabBar("network_tabbar"))
		{
			if (ImGui::BeginTabItem("Network"))
			{
				view::network();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Network"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spoofing"))
			{
				view::spoofing();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Spoofing"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Missions"))
			{
				view::missions();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Missions"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Player Database"))
			{
				view::player_database();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_PLAYER_DB"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Session Browser"))
			{
				view::session_browser();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Session Browser"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Stat Editor"))
			{
				view::stat_editor();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Stat Editor"_J);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
