/**
 * @file view_world_tabs.cpp
 * @brief World options tab bar.
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
	void view::world_tabs()
	{
		if (ImGui::BeginTabBar("world_tabbar"))
		{
			if (ImGui::BeginTabItem("Time And Weather"))
			{
				view::time_and_weather();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spawn Ped"))
			{
				view::spawn_ped();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Embient Population"))
			{
				view::ambient();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Creator"))
			{
				view::creator();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Blackhole"))
			{
				view::blackhole();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Model Swapper"))
			{
				view::model_swapper();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Nearby"))
			{
				view::nearby();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
