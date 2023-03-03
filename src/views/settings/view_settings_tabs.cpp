#include "pointers.hpp"
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
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Hotkey Settings"))
			{
				view::hotkey_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Context Menu"))
			{
				view::context_menu_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("ESP"))
			{
				view::esp_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI"))
			{
				view::gui_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Translation"))
			{
				view::translation_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Reactions"))
			{
				view::reaction_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Protection"))
			{
				view::protection_settings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Stat Editor"))
			{
				view::stat_editor();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
}
