#include "views/view.hpp"

namespace big
{
	void view::vfx()
	{
		ImGui::Checkbox("Enable Custom Sky Color", &g.vfx.enable_custom_sky_color);

		ImGui::ColorEdit4("Azimuth East", (float*)&g.vfx.azimuth_east);
		ImGui::ColorEdit4("Azimuth West", (float*)&g.vfx.azimuth_west);
		ImGui::ColorEdit4("Azimuth Transition", (float*)&g.vfx.azimuth_transition);
		ImGui::ColorEdit4("Zenith", (float*)&g.vfx.zenith);

		ImGui::Separator();

		ImGui::SliderFloat("Stars Intensity", &g.vfx.stars_intensity, 0, 20);
	}
}