/**
 * @file view_nearby.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "util/entity.hpp"
#include "util/local_player.hpp"
#include "util/ped.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

namespace big
{
	void view::nearby()
	{
		ImGui::Text("Peds");
		// Nearby Ped Actions
		components::button<ImVec2(110, 0)>("Delete", [] {
			for (auto peds : entity::get_entities(false, true))
			{
				entity::delete_entity(peds);
			}
		});
		ImGui::SameLine();

		components::button<ImVec2(110, 0), ImVec4(0.70196f, 0.3333f, 0.00392f, 1.f)>("Kill", [] {
			for (auto peds : entity::get_entities(false, true))
			{
				ped::kill_ped(peds);
			}
		});
		ImGui::SameLine();

		components::button<ImVec2(110, 0), ImVec4(0.76078f, 0.f, 0.03529f, 1.f)>("Kill Enemies", [] {
			for (auto peds : entity::get_entities(false, true))
			{
				ped::kill_ped_by_relation(peds, 4 || 5);
			}
		});

		// Nearby Ped Loops / Toggles
		components::command_checkbox<"pedsignore">();
		ImGui::SameLine(140.f);
		components::command_checkbox<"pedrain">();
		ImGui::SameLine(265.f);
		components::command_checkbox<"riotmode">();
		components::command_checkbox<"highalert">();
		ImGui::SameLine(140.f);
		components::command_checkbox<"pedrush">();

		ImGui::SeparatorText("Vehicles");
		// Nearby Vehicle Actions

		components::button<ImVec2(110, 0), ImVec4(0.02745f, 0.4745f, 0.10196f, 1.f)>("Max Upgrade", [] {
			for (auto vehs : entity::get_entities(true, false))
			{
				if (entity::take_control_of(vehs))
				{
					vehicle::max_vehicle(vehs);
					script::get_current()->yield();
				}
			}
		});
		ImGui::SameLine();

		components::button<ImVec2(110, 0), ImVec4(0.4549f, 0.03529f, 0.03529f, 1.f)>("Downgrade", [] {
			for (auto vehs : entity::get_entities(true, false))
			{
				if (entity::take_control_of(vehs))
				{
					vehicle::downgrade(vehs);
					script::get_current()->yield();
				}
			}
		});

		components::command_checkbox<"vehiclerain">();
	}
}