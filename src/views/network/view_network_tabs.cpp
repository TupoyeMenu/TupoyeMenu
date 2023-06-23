#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::network_tabs()
	{
		if (ImGui::BeginTabBar("network_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_NETWORK"_T.data()))
			{
				view::network();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_NETWORK"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SPOOFING"_T.data()))
			{
				view::spoofing();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SPOOFING"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_MISSIONS"_T.data()))
			{
				view::missions();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_MISSIONS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PLAYER_DB"_T.data()))
			{
				view::player_database();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_PLAYER_DB"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SESSION_BROWSER"_T.data()))
			{
				view::session_browser();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SESSION_BROWSER"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_STAT_EDITOR"_T.data()))
			{
				view::stat_editor();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_STAT_EDITOR"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
