#include "pointers.hpp"
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
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_HOTKEYS"_T.data()))
			{
				view::hotkey_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_CONTEXT_MENU"_T.data()))
			{
				view::context_menu_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_ESP"_T.data()))
			{
				view::esp_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_GUI"_T.data()))
			{
				view::gui_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_TRANSLATION"_T.data()))
			{
				view::translation_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_REACTIONS"_T.data()))
			{
				view::reaction_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PROTECTION"_T.data()))
			{
				view::protection_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GTA Cache"))
			{
				view::gta_cache();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
