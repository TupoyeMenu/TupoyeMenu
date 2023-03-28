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
			ImGui::ListBoxFooter();
		}

	}
}
