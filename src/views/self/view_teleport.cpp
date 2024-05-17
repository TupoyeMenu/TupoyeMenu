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
		ImGui::TextUnformatted("Blips:");

		components::command_button<"waypointtp">({}, "Waypoint");
		ImGui::SameLine();
		components::command_button<"objectivetp">({}, "Objective");
		ImGui::SameLine();
		components::command_button<"highlighttp">({}, "Selected");
		components::command_checkbox<"autotptowp">();

		ImGui::SeparatorText("Movement");

		components::small_text("Current coordinates");
		float coords[3] = {self::pos.x, self::pos.y, self::pos.z};
		static float new_location[3];
		static float increment = 1;

		ImGui::SetNextItemWidth(400);
		ImGui::InputFloat3("##currentcoordinates", coords, "%f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		components::button("Copy to custom", [coords] {
			std::copy(std::begin(coords), std::end(coords), std::begin(new_location));
		});

		components::small_text("Custom Teleport");
		ImGui::SetNextItemWidth(400);
		ImGui::InputFloat3("##Customlocation", new_location);
		ImGui::SameLine();
		components::button("Teleport", [] {
			teleport::to_coords({new_location[0], new_location[1], new_location[2]});
		});

		ImGui::Spacing();
		components::small_text("Specific movement");
		ImGui::Spacing();

		ImGui::SetNextItemWidth(200);
		ImGui::InputFloat("Distance", &increment);

		ImGui::BeginGroup();
		components::button("Forward", [] {
			teleport::to_coords(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, increment, 0));
		});
		components::button("Backward", [] {
			teleport::to_coords(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, -increment, 0));
		});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("Left", [] {
			teleport::to_coords(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, -increment, 0, 0));
		});
		components::button("Right", [] {
			teleport::to_coords(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, increment, 0, 0));
		});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("Up", [] {
			teleport::to_coords(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, 0, increment));
		});
		components::button("Down", [] {
			teleport::to_coords(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0, 0, -increment));
		});
		ImGui::EndGroup();

		ImGui::SeparatorText("Vehicles:");

		components::command_button<"lastvehtp">();
		ImGui::SameLine();
		components::command_button<"bringpv">();
		ImGui::SameLine();
		components::command_button<"pvtp">();

		ImGui::SeparatorText("IPL");

		static int current_select = 0;
		static int last_select    = -1;

		ImGui::SetNextItemWidth(400);
		if (ImGui::BeginCombo("##Ipllocation", ipls[current_select].friendly_name))
		{
			for (int i = 0; i < IM_ARRAYSIZE(ipls); i++)
			{
				bool is_selected = (i == current_select);
				if (ImGui::Selectable(ipls[i].friendly_name, is_selected))
				{
					current_select = i;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		components::button("Load IPL", [] 
		{
			// If we've changed selections, first unload previously loaded IPL, then load previously deleted IPLs
			if (current_select != last_select)
			{
				// Unload previously loaded IPL of the last selection
				// If this is our first time loading an IPL (especially the first one in the list, then don't unload anything)
				if (last_select != -1)
				{
					for (auto& ipl_name_unload : ipls[last_select].ipl_names)
					{
						if (STREAMING::IS_IPL_ACTIVE(ipl_name_unload))
						{
							STREAMING::REMOVE_IPL(ipl_name_unload);
						}
					}

					// Load previously deleted IPLs of the last selection
					for (auto& ipl_name_load : ipls[last_select].ipl_names_remove)
					{
						STREAMING::REQUEST_IPL(ipl_name_load);
					}
				}

				// Load new IPLs of the current selection
				for (auto& ipl_name : ipls[current_select].ipl_names)
				{
					STREAMING::REQUEST_IPL(ipl_name);
				}

				// Remove old IPLs of the current selection to avoid conflicts
				for (auto& ipl_name_remove : ipls[current_select].ipl_names_remove_when_load)
				{
					STREAMING::REMOVE_IPL(ipl_name_remove);
				}

				last_select = current_select;
			}
		});
		ImGui::SameLine();
		components::button("Teleport to IPL", [] 
		{
			teleport::to_coords(ipls[current_select].location);
		});

		ImGui::Spacing();
		components::small_text("IPL Information");

		ImGui::Text("IPL Count %zu", ipls[current_select].ipl_names.size());
		ImGui::Text("Position X: %f | Y: %f | Z: %f", ipls[current_select].location.x, ipls[current_select].location.y, ipls[current_select].location.z);
	}
}
