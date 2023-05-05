/**
 * @file view_teleport.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/ipls.hpp"
#include "fiber_pool.hpp"
#include "imgui.h"
#include "util/globals.hpp"
#include "util/mobile.hpp"
#include "util/teleport.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

namespace big
{
	void view::teleport()
	{
		ImGui::Text("Blips:");

		components::command_button<"waypointtp">({}, "Waypoint");
		ImGui::SameLine();
		components::command_button<"objectivetp">({}, "Objective");
		components::command_checkbox<"autotptowp">();

		ImGui::SeparatorText("Vehicles:");
		components::command_button<"lastvehtp">();
		ImGui::SameLine();
		components::command_button<"bringpv">();
		ImGui::SameLine();
		components::command_button<"pvtp">();

		ImGui::SeparatorText("IPL");

		if (ImGui::BeginCombo("IPL Location", ipls[g.self.ipls.select].friendly_name))
		{
			for (int i = 0; i < IM_ARRAYSIZE(ipls); i++)
			{
				if (ImGui::Selectable(ipls[i].friendly_name, i == g.self.ipls.select))
					g.self.ipls.select = i;

				if (i == g.self.ipls.select)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		auto selected_ipl = ipls[g.self.ipls.select];
		if (components::button("Load IPL"))
		{
			//unload all previous ipls
			for (auto& ipl : ipls)
				for (auto& ipl_name : ipl.ipl_names)
				{
					if (STREAMING::IS_IPL_ACTIVE(ipl_name))
					{
						LOG(INFO) << "unloading existing ipl " << ipl_name;
						STREAMING::REMOVE_IPL(ipl_name);
					}
				}

			//load the new ipl
			for (auto& ipl_name : selected_ipl.ipl_names)
				STREAMING::REQUEST_IPL(ipl_name);
		}

		ImGui::SameLine();

		if (components::button("Teleport to IPL"))
		{
			PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped,
			    selected_ipl.location.x,
			    selected_ipl.location.y,
			    selected_ipl.location.z);
		}

		components::sub_title("IPL Informations");
		ImGui::Text(std::vformat("IPL Count {}", std::make_format_args(selected_ipl.ipl_names.size())).data());
		ImGui::Text(std::vformat("Position X: {} | Y: {} | Z: {}",
		    std::make_format_args(selected_ipl.location.x, selected_ipl.location.y, selected_ipl.location.z))
		                .data());

		ImGui::SeparatorText("Manual:");
		
		static float coords[3] = { 0.f, 0.f, 0.f }; // This is shit
		ImGui::InputFloat3("Coords", coords);

		components::button("Teleport to Coords", [] {
			Vector3 coords_v;
			coords_v.x = coords[1];
			coords_v.y = coords[2];
			coords_v.z = coords[3];
			teleport::to_coords(coords_v);
		});
	}
}
