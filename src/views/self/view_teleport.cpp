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

		ImGui::SeparatorText("Movement");

		static float new_location[3];
		static float increment = 1;

		components::small_text("Custom teleport");
		ImGui::SetNextItemWidth(200);
		ImGui::InputFloat3("##Customlocation", new_location);
		ImGui::SameLine();
		components::button("Teleport", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), {new_location[0], new_location[1], new_location[2]});
		});

		ImGui::Spacing();
		components::small_text("Specific movement");
		ImGui::SetNextItemWidth(200);
		ImGui::InputFloat("Distance", &increment);

		ImGui::BeginGroup();
		components::button("Forward", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, increment, 0));
		});
		components::button("Backward", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, -increment, 0));
		});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("Left", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, -increment, 0, 0));
		});
		components::button("Right", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, increment, 0, 0));
		});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("Up", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, 0, increment));
		});
		components::button("Down", [] {
			teleport::teleport_player_to_coords(g_player_service->get_self(), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, 0, -increment));
		});
		ImGui::EndGroup();

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

		ImGui::Spacing();
		components::small_text("IPL Informations");

		ImGui::Text(std::format("IPL Count {}", selected_ipl.ipl_names.size()).data());
		ImGui::Text(std::format("Position X: {} | Y: {} | Z: {}",
		    selected_ipl.location.x,
		    selected_ipl.location.y,
		    selected_ipl.location.z)
		                .data());
	}
}
