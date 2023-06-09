#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::settings_tabs()
	{
		if (ImGui::BeginTabBar("settings_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_SETTINGS"_T.data()))
			{
				view::settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SETTINGS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_HOTKEYS"_T.data()))
			{
				view::hotkey_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_HOTKEYS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_LUA_SCRIPTS"_T.data()))
			{
				view::lua_scripts();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_LUA_SCRIPTS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_CONTEXT_MENU"_T.data()))
			{
				view::context_menu_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_CONTEXT_MENU"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_ESP"_T.data()))
			{
				view::esp_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_ESP"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_GUI"_T.data()))
			{
				view::gui_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_GUI"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_TRANSLATION"_T.data()))
			{
				view::translation_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_TRANSLATION"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_REACTIONS"_T.data()))
			{
				view::reaction_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_REACTIONS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PROTECTION"_T.data()))
			{
				view::protection_settings();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_PROTECTION"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GTA Cache"))
			{
				view::gta_cache();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GTA Cache"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
