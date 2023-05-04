/**
 * @file view_ambient.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "imgui.h"
#include "views/view.hpp"

namespace big
{
	void view::ambient()
	{
		ImGui::Checkbox("Override Ped Population", &g.session.population_control.ped_enable);

		ImGui::BeginDisabled(!g.session.population_control.ped_enable);
		ImGui::SliderInt("Ped Population", &g.session.population_control.ped, 0, 3);
		ImGui::EndDisabled();


		ImGui::Checkbox("Override Vehicle Population", &g.session.population_control.vehicle_enable);

		ImGui::BeginDisabled(!g.session.population_control.vehicle_enable);
		ImGui::SliderInt("Vehicle Population", &g.session.population_control.vehicle, 0, 3);
		ImGui::EndDisabled();
	}
}
