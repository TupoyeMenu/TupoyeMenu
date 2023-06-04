/**
 * @file view_time_and_weather.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "fiber_pool.hpp"
#include "imgui.h"
#include "util/session.hpp"
#include "views/view.hpp"

namespace big
{
	void view::time_and_weather()
	{
		ImGui::Text("Local Time");
		ImGui::Checkbox("Override Time", &g.session.override_time);

		if (g.session.override_time)
		{
			ImGui::SliderInt("Hour", &g.session.custom_time.hour, 0, 23);
			ImGui::SliderInt("Minute", &g.session.custom_time.minute, 0, 59);
			ImGui::SliderInt("Second", &g.session.custom_time.second, 0, 59);
		}

		components::button("Clear Override", [] {
			MISC::CLEAR_OVERRIDE_WEATHER();
		});

		ImGui::SeparatorText("Local Weather");
		if (ImGui::ListBox("##weather-listbox", &g.session.local_weather, session::weathers, 15))
		{
			g_fiber_pool->queue_job([] {
				session::local_weather();
			});
			ImGui::EndListBox();
		}

	}
}
