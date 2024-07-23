/**
 * @file view_player_tabs.cpp
 * @brief Player options tab bar.
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
						g_lua_manager->draw_gui("INFO"_J);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Troll"))
				{
					view::view_player_troll();
					if (g_lua_manager)
						g_lua_manager->draw_gui("TROLL"_J);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Vehicle"))
				{
					view::view_player_vehicle();
					if (g_lua_manager)
						g_lua_manager->draw_gui("VEHICLE"_J);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Misc"))
				{
					view::view_player_misc();
					if (g_lua_manager)
						g_lua_manager->draw_gui("MISC"_J);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Toxic"))
				{
					view::view_player_toxic();
					if (g_lua_manager)
						g_lua_manager->draw_gui("TOXIC"_J);
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
						g_lua_manager->draw_gui("KICK"_J);
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}
}
