/**
 * @file view_vehicle_tabs.cpp
 * @brief Vehicle options tab bar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::vehicle_tabs()
	{
		if (ImGui::BeginTabBar("vehicle_tabbar"))
		{
			if (ImGui::BeginTabItem("Vehicle"))
			{
				view::vehicle();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("LS Customs"))
			{
				view::lsc();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spawn Vehicle"))
			{
				view::spawn_vehicle();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Personal Vehicle"))
			{
				view::pv();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Fun Features"))
			{
				view::fun_vehicle();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
