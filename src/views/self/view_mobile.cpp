/**
 * @file view_mobile.cpp
 * @brief Options from an in game mobile phone.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/phone_types.hpp"
#include "fiber_pool.hpp"
#include "services/mobile/mobile_service.hpp"
#include "util/mobile.hpp"
#include "views/view.hpp"

namespace big
{
	void view::mobile()
	{
		ImGui::Text("Mors Mutual");

		components::button("Mors Mutual Fix All Vehicles", [] {
			int amount_fixed = mobile::mors_mutual::fix_all();
			g_notification_service->push("Mobile", std::format("{} vehicle{} been fixed.", amount_fixed, amount_fixed == 1 ? " has" : "s have"));
		});

		ImGui::SeparatorText("CEO Abilities");

		components::button("Bullshark Testosterone", [] {
			mobile::ceo_abilities::request_bullshark_testosterone();
		});

		components::command_button<"ballisticarmor">();

		ImGui::SeparatorText("Merryweather");

		components::button("Request Ammo Drop", [] {
			mobile::merry_weather::request_ammo_drop();
		});

		components::button("Helicopter Pickup", [] {
			mobile::merry_weather::request_helicopter_pickup();
		});

		components::command_button<"boatpickup">();

		components::button("Request Backup Helicopter", [] {
			mobile::merry_weather::request_backup_helicopter();
		});

		components::button("Request Airstrike", [] {
			mobile::merry_weather::request_airstrike();
		});		
	}
}