/**
 * @file view_time_and_weather.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */
#include "core/data/weathers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::time_and_weather()
	{
		components::command_checkbox<"timeoverride">();

		if (g.world.custom_time.override_time)
		{
			ImGui::SliderInt("Hour", &g.world.custom_time.hour, 0, 23);
			ImGui::SliderInt("Minute", &g.world.custom_time.minute, 0, 59);
			ImGui::SliderInt("Second", &g.world.custom_time.second, 0, 59);
		}

		components::command_checkbox<"weatheroverride">();

		if (g.world.override_weather)
		{
			if (ImGui::BeginCombo("Weather", weathers[g.world.local_weather]))
			{
				for (int i = 0; i < weathers.size(); i++)
				{
					if (ImGui::Selectable(weathers[i], g.world.local_weather == i))
					{
						g.world.local_weather = i;
					}

					if (g.world.local_weather == i)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}
		}

		components::command_checkbox<"blackout">();
	}
}
