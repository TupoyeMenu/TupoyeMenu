/**
 * @file view_settings_tabs.cpp
 * @brief Settings tab bar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

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
			if (ImGui::BeginTabItem("Hotkeys"))
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
			if (ImGui::BeginTabItem("GTA Cache"))
			{
				view::gta_cache();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
