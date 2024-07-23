/**
 * @file view_settings_tabs.cpp
 * @brief Settings tab bar.
 */

#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::settings_tabs()
	{
		if (ImGui::BeginTabBar("settings_tabbar"))
		{
			if (ImGui::BeginTabItem("Settings"))
			{
				view::settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_SETTINGS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Hotkeys"))
			{
				view::hotkey_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_HOTKEY_SETTINGS"_J);
				ImGui::EndTabItem();
			}
#if defined (ENABLE_LUA)
			if (ImGui::BeginTabItem("Lua Scripts"))
			{
				view::lua_scripts();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_LUA_SCRIPTS"_J);
				ImGui::EndTabItem();
			}
#endif // ENABLE_LUA
			if (ImGui::BeginTabItem("Context Menu"))
			{
				view::context_menu_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_CONTEXT_MENU_SETTINGS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("ESP"))
			{
				view::esp_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_ESP_SETTINGS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI"))
			{
				view::gui_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_GUI_SETTINGS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Reactions"))
			{
				view::reaction_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_REACTION_SETTINGS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Protection"))
			{
				view::protection_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_PROTECTION_SETTINGS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GTA Cache"))
			{
				view::gta_cache();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_GTA_CACHE_SETTINGS"_J);
				ImGui::EndTabItem();
			}

			if (g_lua_manager)
				g_lua_manager->draw_child_tabs_external("GUI_TAB_SETTINGS"_J);

			ImGui::EndTabBar();
		}
	}
}
