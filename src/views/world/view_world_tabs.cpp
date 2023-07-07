/**
 * @file view_world_tabs.cpp
 * @brief World options tab bar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::world_tabs()
	{
		if (ImGui::BeginTabBar("world_tabbar"))
		{
			if(ImGui::BeginTabItem("World"))
			{
				view::world();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_WORLD"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Time And Weather"))
			{
				view::time_and_weather();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_TIME_N_WEATHER"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spawn Ped"))
			{
				view::spawn_ped();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SPAWN_PED"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Embient Population"))
			{
				view::ambient();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Embient Population"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Creator"))
			{
				view::creator();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_CREATOR"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Blackhole"))
			{
				view::blackhole();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_BLACKHOLE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Model Swapper"))
			{
				view::model_swapper();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_MODEL_SWAPPER"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
