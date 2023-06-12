/**
 * @file view_player_tabs.cpp
 * @brief Player options tab bar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "pointers.hpp"
#include "services/players/player_service.hpp"
#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_tabs()
	{
		if (!*g_pointers->m_gta.m_is_session_started && !g_player_service->get_selected()->is_valid())
			return;

		if (ImGui::Begin("Player", &g.window.player))
		{
			if (ImGui::BeginTabBar("player_tabbar"))
			{
				if (ImGui::BeginTabItem("Info"))
				{
					view::view_player_info();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("INFO"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Troll"))
				{
					view::view_player_troll();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("TROLL"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Vehicle"))
				{
					view::view_player_vehicle();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("VEHICLE"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Misc"))
				{
					view::view_player_misc();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("MISC"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Toxic"))
				{
					view::view_player_toxic();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("TOXIC"));
					ImGui::EndTabItem();
				}

#ifdef ENABLE_CRASHES
				if (ImGui::BeginTabItem("Kicks and Crashes"))
#else
				if (ImGui::BeginTabItem("Kicks"))
#endif // ENABLE_CRASHES
				{
					view::view_player_kicks_and_crashes();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("KICK"));
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}
}
