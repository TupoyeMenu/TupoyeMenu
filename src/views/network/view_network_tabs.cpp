/**
 * @file view_network_tabs.cpp
 * @brief Network options tab bar.
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
			if (ImGui::BeginTabItem("Chat"))
			{
				view::chat();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Chat"_J);
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
