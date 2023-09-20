/**
 * @file view_pv.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "fiber_pool.hpp"
#include "natives.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "services/mobile/mobile_service.hpp"
#include "services/model_preview/model_preview_service.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

namespace big
{
	bool is_blacklist_vehicle(Hash vehicle_hash)
	{
		switch (vehicle_hash)
		{
			case RAGE_JOAAT("avenger"):
			case RAGE_JOAAT("avenger3"):
			case RAGE_JOAAT("hauler2"):
			case RAGE_JOAAT("phantom3"):
			case RAGE_JOAAT("trailersmall2"):
			case RAGE_JOAAT("khanjali"):
			case RAGE_JOAAT("chernobog"):
			case RAGE_JOAAT("riot2"):
			case RAGE_JOAAT("thruster"):
			case RAGE_JOAAT("brickade2"):
			case RAGE_JOAAT("manchez3"):
			case RAGE_JOAAT("terbyte"):
			case RAGE_JOAAT("speedo4"):
			case RAGE_JOAAT("mule4"):
			case RAGE_JOAAT("pounder2"):
			case RAGE_JOAAT("rcbandito"):
			case RAGE_JOAAT("minitank"):
				return true;
		}
		return false;
	}

	void view::pv()
	{
		if (ImGui::Checkbox("Preview", &g.clone_pv.preview_vehicle))
		{
			if (!g.clone_pv.preview_vehicle)
			{
				g_model_preview_service->stop_preview();
			}
		}
		ImGui::SameLine();
		components::help_marker("Shows a render of what you have highlighted in front of you.");
		ImGui::SameLine();
		ImGui::Checkbox("Spawn Inside", &g.clone_pv.spawn_inside);
		ImGui::SameLine();
		components::help_marker("Controls whether the player should be set inside the vehicle after it spawns");
		ImGui::SameLine();

		static char plate_buf[9] = {0};

		ImGui::SameLine();
		ImGui::Checkbox("Spawn Clone", &g.clone_pv.spawn_clone);
		ImGui::SameLine();
		components::help_marker("Determines if the vehicle selected to be spawned will be set to your personal vehicle.");
		if (g.clone_pv.spawn_clone)
		{
			ImGui::Checkbox("Spawn Maxed", &g.clone_pv.spawn_maxed);
			components::help_marker("Controls whether the vehicle spawned will have its mods maxed out");

			ImGui::SameLine();
			ImGui::Checkbox("Clone PV Plate", &g.clone_pv.clone_plate);
			if (!g.clone_pv.clone_plate)
			{
				ImGui::SetNextItemWidth(300.f);

				strncpy(plate_buf, g.clone_pv.plate.c_str(), 9);
				components::input_text_with_hint("Plate", "Plate Number", plate_buf, sizeof(plate_buf), ImGuiInputTextFlags_None, [] {
					g.clone_pv.plate = plate_buf;
				});
			}
		}


		static int selected_class = -1;
		const auto& class_arr     = g_gta_data_service->vehicle_classes();

		ImGui::SetNextItemWidth(300.f);
		if (ImGui::BeginCombo("Vehicle Class", selected_class == -1 ? "All" : class_arr[selected_class].c_str()))
		{
			if (ImGui::Selectable("All", selected_class == -1))
			{
				selected_class = -1;
			}

			for (int i = 0; i < class_arr.size(); i++)
			{
				if (ImGui::Selectable(class_arr[i].c_str(), selected_class == i))
				{
					selected_class = i;
				}

				if (selected_class == i)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}


		static char search[64];

		ImGui::SetNextItemWidth(300.f);
		components::input_text_with_hint("Model Name", "Search", search, sizeof(search), ImGuiInputTextFlags_None);

		g_mobile_service->refresh_personal_vehicles();

		auto num_of_rows = 0;

		std::set<int> indexes_to_use;

		if (!g_mobile_service->personal_vehicles().empty())
		{
			std::string lower_search = search;
			std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), tolower);

			for (const auto& it : g_mobile_service->personal_vehicles())
			{
				const auto& label        = it.first;
				const auto& personal_veh = it.second;
				const auto& item         = g_gta_data_service->vehicle_by_hash(personal_veh->get_hash());

				std::string vehicle_class        = item.m_vehicle_class;
				std::string display_name         = label;
				std::string display_manufacturer = item.m_display_manufacturer;
				std::transform(display_name.begin(), display_name.end(), display_name.begin(), ::tolower);
				std::transform(display_manufacturer.begin(), display_manufacturer.end(), display_manufacturer.begin(), ::tolower);

				if ((selected_class == -1 || class_arr[selected_class] == vehicle_class)
				    && (display_name.find(lower_search) != std::string::npos || display_manufacturer.find(lower_search) != std::string::npos))
				{
					if (!g.clone_pv.spawn_clone && is_blacklist_vehicle(personal_veh->get_hash()))
					{
						continue;
					}
					indexes_to_use.insert(personal_veh->get_id());
				}
			}
			num_of_rows = indexes_to_use.size();
		}
		else
		{
			num_of_rows = 2;
		}

		static const auto over_30 = (30 * ImGui::GetTextLineHeightWithSpacing() + 2);
		const auto box_height = num_of_rows <= 30 ? (num_of_rows * ImGui::GetTextLineHeightWithSpacing() + 2) : over_30;
		if (ImGui::BeginListBox("###personal_veh_list", {300, box_height}))
		{
			if (g_mobile_service->personal_vehicles().empty())
			{
				ImGui::Text("No personal vehicles found, \nare you online?");
			}
			else
			{
				std::string lower_search = search;
				std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), tolower);

				for (const auto& it : g_mobile_service->personal_vehicles())
				{
					const auto& personal_veh = it.second;

					if (indexes_to_use.contains(personal_veh->get_id()))
					{
						const auto& label = it.first;

						ImGui::PushID('v' << 24 & personal_veh->get_id());
						components::selectable(label, false, [&personal_veh] {
							if (g.clone_pv.spawn_clone)
							{
								Vector3 spawn_location =
								    vehicle::get_spawn_location(g.spawn_vehicle.spawn_inside, personal_veh->get_hash());
								float spawn_heading = ENTITY::GET_ENTITY_HEADING(self::ped);

								auto vehicle_idx = personal_veh->get_vehicle_idx();
								auto owned_mods  = vehicle::get_owned_mods_from_vehicle_idx(vehicle_idx);

								const char* spawn_plate_buf = plate_buf;
								if (g.clone_pv.clone_plate)
								{
									spawn_plate_buf = personal_veh->get_plate();
								}

								auto veh = vehicle::clone_from_owned_mods(owned_mods, spawn_location, spawn_heading);

								if (veh == 0)
								{
									g_notification_service->push_error("Vehicle", "Unable to spawn vehicle");
								}
								else
								{
									if (g.clone_pv.spawn_maxed)
									{
										vehicle::max_vehicle(veh);
									}

									vehicle::set_plate(veh, spawn_plate_buf);

									if (g.clone_pv.spawn_inside)
									{
										vehicle::teleport_into_vehicle(veh);
									}
								}
								// cleanup clones
								ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
							}
							else
							{
								strcpy(search, "");
								personal_veh->summon();
							}

							g_model_preview_service->stop_preview();
						});
						ImGui::PopID();

						if (!g.clone_pv.preview_vehicle || (g.clone_pv.preview_vehicle && !ImGui::IsAnyItemHovered()))
						{
							g_model_preview_service->stop_preview();
						}
						else if (ImGui::IsItemHovered())
						{
							g_fiber_pool->queue_job([&personal_veh] {
								g_model_preview_service->show_vehicle(
								    vehicle::get_owned_mods_from_vehicle_idx(personal_veh->get_vehicle_idx()),
								    g.clone_pv.spawn_maxed);
							});
						}
					}
				}
			}

			ImGui::EndListBox();
		}
	}
}
