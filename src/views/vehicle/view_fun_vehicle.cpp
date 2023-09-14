/**
 * @file view_fun_vehicle.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/bullet_impact_types.hpp"
#include "core/data/special_ammo_types.hpp"
#include "core/data/speed_units.hpp"
#include "core/enums.hpp"
#include "fiber_pool.hpp"
#include "script.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "services/model_preview/model_preview_service.hpp"
#include "util/mobile.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

#include <imgui_internal.h>

namespace big
{
	void view::fun_vehicle()
	{
		ImGui::Text("Seat Changer");
		{
			static std::map<int, bool> seats;
			static bool ready = true;

			if (self::veh == 0)
			{
				seats.clear();
			}

			if (self::veh != 0 && ready == true)
			{
				ready = false;

				g_fiber_pool->queue_job([] {
					std::map<int, bool> tmp_seats;

					int num_of_seats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(self::veh);

					for (int i = -1; i < num_of_seats; i++)
					{
						tmp_seats[i] = VEHICLE::IS_VEHICLE_SEAT_FREE(self::veh, i, true);
					}

					seats = tmp_seats;
					ready = true;
				});
			}

			if (seats.size() == 0)
			{
				ImGui::Text("Please enter a vehicle.");
			}
			else
			{
				for (auto& it : seats)
				{
					int idx = it.first;

					if (!it.second)
					{
						ImGui::BeginDisabled();
					}

					std::string name = "Driver";

					if (idx >= 0)
					{
						name = "Seat " + std::to_string(idx + 1);
					}

					if ((idx + 1) % 4 != 0)
					{
						ImGui::SameLine();
					}

					components::button(name, [idx] {
						PED::SET_PED_INTO_VEHICLE(self::ped, self::veh, idx);
					});
					if (!it.second)
					{
						ImGui::EndDisabled();
					}
				}
			}
		}
		ImGui::SeparatorText("Auto Drive");

		{
			float auto_drive_speed_user_unit = vehicle::mps_to_speed(g.vehicle.auto_drive_speed, g.vehicle.speed_unit);
			if (ImGui::SliderFloat(std::format("Top Speed({})", speed_unit_strings[(int)g.vehicle.speed_unit]).c_str(),
			        &auto_drive_speed_user_unit,
			        vehicle::mps_to_speed(0.f, g.vehicle.speed_unit),
			        vehicle::mps_to_speed(150.f, g.vehicle.speed_unit),
			        "%.1f"))
			{
				g.vehicle.auto_drive_speed = vehicle::speed_to_mps(auto_drive_speed_user_unit, g.vehicle.speed_unit);
			}

			const char* driving_style_names[] = {"Law-Abiding", "The Road Is Yours"};
			if (ImGui::BeginCombo("Driving Style", driving_style_names[(int)g.vehicle.auto_drive_style]))
			{
				for (int i = 0; i < 2; i++)
				{
					if (ImGui::Selectable(driving_style_names[i], g.vehicle.auto_drive_style == (AutoDriveStyle)i))
					{
						g.vehicle.auto_drive_style = (AutoDriveStyle)i;
						g_notification_service->push_success("Auto Drive", std::format("Driving style set to {}.", driving_style_names[i]));
					}

					if (g.vehicle.auto_drive_style == (AutoDriveStyle)i)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (components::button("To Objective"))
				g.vehicle.auto_drive_destination = AutoDriveDestination::OBJECTITVE;

			ImGui::SameLine();

			if (components::button("To Waypoint"))
				g.vehicle.auto_drive_destination = AutoDriveDestination::WAYPOINT;

			ImGui::SameLine();

			if (components::button("Wander"))
				g.vehicle.auto_drive_destination = AutoDriveDestination::WANDER;

			ImGui::SameLine();

			if (components::button("Emergency Stop"))
				g.vehicle.auto_drive_destination = AutoDriveDestination::EMERGENCY_STOP;
		}

		ImGui::SeparatorText("Dirt Level");
		{
			if (g_local_player == nullptr || g_local_player->m_vehicle == nullptr)
			{
				ImGui::Text("Please enter a vehicle.");
			}
			else if (g.vehicle.keep_vehicle_clean)
			{
				ImGui::Text("Keep Vehicle Clean is turned on, disable it to change the dirt level.");
			}
			else if (g.vehicle.keep_vehicle_repaired)
			{
				ImGui::Text("Keep Vehicle Repaired is turned on, disable it to change the vehicle health.");
			}
			else
			{
				ImGui::SliderFloat("Dirt Level", &g_local_player->m_vehicle->m_dirt_level, 0.f, 15.f, "%.1f");
			}
		}

		ImGui::SeparatorText("Rainbow Paint");
		{
			components::command_checkbox<"rainbowpri">("Primary");
			ImGui::SameLine();
			components::command_checkbox<"rainbowsec">("Secondary");
			ImGui::SameLine();
			components::command_checkbox<"rainbowneons">("Neon");
			ImGui::SameLine();
			components::command_checkbox<"rainbowsmoke">("Smoke");

			const char* rgb_types[] = {"Off", "Fade", "Spasm"};

			ImGui::SetNextItemWidth(120);
			if (ImGui::BeginCombo("RGB Type", rgb_types[(int)g.vehicle.rainbow_paint.type]))
			{
				for (int i = 0; i < 3; i++)
				{
					bool itemSelected = (int)g.vehicle.rainbow_paint.type == i;

					if (ImGui::Selectable(rgb_types[i], itemSelected))
					{
						g.vehicle.rainbow_paint.type = (RainbowPaintType)i;
					}

					if (itemSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}
			if (g.vehicle.rainbow_paint.type != RainbowPaintType::Off)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				components::command_int_slider<"rainbowspeed">("RGB Speed");
			}
		}
		ImGui::SeparatorText("Boost");

		const char* boost_behaviors[] = {"Default", "Instant Refill", "Infinite", "Hold for Boost"};
		if (ImGui::BeginCombo("Boost Behavior", boost_behaviors[static_cast<int>(g.vehicle.boost_behavior)]))
		{
			for (int i = 0; i < 4; i++)
			{
				bool itemSelected = g.vehicle.boost_behavior == static_cast<eBoostBehaviors>(i);

				if (ImGui::Selectable(boost_behaviors[i], itemSelected))
				{
					g.vehicle.boost_behavior = static_cast<eBoostBehaviors>(i);
				}

				if (itemSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		ImGui::SeparatorText("Vehicle Fly");
		{
			ImGui::BeginGroup();

			components::command_checkbox<"vehiclefly">("Enabled");
			ImGui::Checkbox("Don't Stop", &g.vehicle.fly.dont_stop);

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Checkbox("Disable Collision", &g.vehicle.fly.no_collision);
			ImGui::Checkbox("Stop On Exit", &g.vehicle.fly.stop_on_exit);

			ImGui::EndGroup();

			float fly_speed_user_unit = vehicle::mps_to_speed(g.vehicle.fly.speed, g.vehicle.speed_unit);
			if (ImGui::SliderFloat(std::format("Speed({})", speed_unit_strings[(int)g.vehicle.speed_unit]).c_str(),
			        &fly_speed_user_unit,
			        vehicle::mps_to_speed(0.f, g.vehicle.speed_unit),
			        vehicle::mps_to_speed(150.f, g.vehicle.speed_unit),
			        "%.1f"))
			{
				g.vehicle.fly.speed = vehicle::speed_to_mps(fly_speed_user_unit, g.vehicle.speed_unit);
			}
		}
		ImGui::SeparatorText("Custom Vehicle Weapons");
		{
			components::command_checkbox<"customvehweaps">();
			components::options_modal("Custom Vehicle Weapons", [] {
				eAmmoSpecialType selected_ammo          = g.vehicle.vehicle_ammo_special.type;
				eExplosionTag selected_explosion        = g.vehicle.vehicle_ammo_special.explosion_tag;
				eExplosionTag selected_rocket_explosion = g.vehicle.vehicle_ammo_special.rocket_explosion_tag;

				ImGui::BeginGroup();
				components::sub_title("Machine Gun");
				if (ImGui::BeginCombo("Special Ammo", SPECIAL_AMMOS[(int)selected_ammo].name))
				{
					for (const auto& special_ammo : SPECIAL_AMMOS)
					{
						if (ImGui::Selectable(special_ammo.name, special_ammo.type == selected_ammo))
						{
							g.vehicle.vehicle_ammo_special.type = special_ammo.type;
						}

						if (special_ammo.type == selected_ammo)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::BeginCombo("Bullet Impact", BULLET_IMPACTS[selected_explosion]))
				{
					for (const auto& [type, name] : BULLET_IMPACTS)
					{
						if (ImGui::Selectable(name, type == selected_explosion))
						{
							g.vehicle.vehicle_ammo_special.explosion_tag = type;
						}

						if (type == selected_explosion)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::InputFloat("Speed", &g.vehicle.vehicle_ammo_special.speed, 10, 100, "%.1f");
				ImGui::InputFloat("Range", &g.vehicle.vehicle_ammo_special.weapon_range, 50, 100, "%.1f");
				ImGui::InputFloat("Time Between Shots", &g.vehicle.vehicle_ammo_special.time_between_shots, 0.001, 0.1, "%.3f");
				ImGui::SameLine();
				components::help_marker("Time taken to fire consecutive shots on the same side. Related to rate of fire.");
				ImGui::InputFloat("Alternate Wait Time", &g.vehicle.vehicle_ammo_special.alternate_wait_time, 0.001, 0.1, "%.3f");
				ImGui::SameLine();
				components::help_marker("Time taken to switch between sides. Related to rate of fire.");
				ImGui::EndGroup();

				ImGui::SameLine();
				ImGui::BeginGroup();
				components::sub_title("Missile");
				if (ImGui::BeginCombo("Explosion##customvehweaps", BULLET_IMPACTS[selected_rocket_explosion]))
				{
					for (const auto& [type, name] : BULLET_IMPACTS)
					{
						if (ImGui::Selectable(name, type == selected_rocket_explosion))
						{
							g.vehicle.vehicle_ammo_special.rocket_explosion_tag = type;
						}

						if (type == selected_rocket_explosion)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::InputFloat("Reload Time", &g.vehicle.vehicle_ammo_special.rocket_reload_time, 0.1, 1, "%.1f");
				ImGui::InputFloat("Speed##rocket", &g.vehicle.vehicle_ammo_special.rocket_launch_speed, 10, 100, "%.1f");
				ImGui::InputFloat("Range##rocket", &g.vehicle.vehicle_ammo_special.rocket_range, 50, 100, "%.1f");
				ImGui::InputFloat("Lock-on Range", &g.vehicle.vehicle_ammo_special.rocket_lock_on_range, 50, 100, "%.1f");
				ImGui::InputFloat("Lock-on Time", &g.vehicle.vehicle_ammo_special.rocket_time_before_homing, 0.01, 0.1, "%.2f");
				ImGui::InputFloat("Time Between Shots##rocket",
				    &g.vehicle.vehicle_ammo_special.rocket_time_between_shots,
				    0.001,
				    0.1,
				    "%.3f");
				ImGui::SameLine();
				components::help_marker("Time taken to fire consecutive shots on the same side. Related to rate of fire.");
				ImGui::InputFloat("Alternate Wait Time##rocket",
				    &g.vehicle.vehicle_ammo_special.rocket_alternate_wait_time,
				    0.001,
				    0.1,
				    "%.3f");
				ImGui::SameLine();
				components::help_marker("Time taken to switch between sides. Related to rate of fire.");
				ImGui::InputFloat("Life Time", &g.vehicle.vehicle_ammo_special.rocket_lifetime, 0.1, 1, "%.1f");
				ImGui::SameLine();
				components::help_marker("Time taken for missile to explode if not hitting anything.");
				ImGui::Checkbox("Smart Missiles", &g.vehicle.vehicle_ammo_special.rocket_improve_tracking);
				ImGui::SameLine();
				components::help_marker("Improves current missile's tracking ability.");
				ImGui::EndGroup();
			});
		}
	}
}
