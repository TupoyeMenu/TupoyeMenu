/**
 * @file view_mobile.cpp
 * @brief Options from an in game mobile phone.
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
		ImGui::TextUnformatted("Mors Mutual");

		components::button("Mors Mutual Fix All Vehicles", [] {
			int amount_fixed = mobile::mors_mutual::fix_all();
			g_notification_service.push_success("Mobile", std::format("{} vehicle{} been fixed.", amount_fixed, amount_fixed == 1 ? " has" : "s have"));
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

		ImGui::SeparatorText("Services");

		components::command_button<"avenger">();
		components::command_button<"kosatka">();
		components::command_button<"moc">();
		components::command_button<"terrorbyte">();
		components::command_button<"acidlab">();
		components::command_button<"acidbike">();

		ImGui::SeparatorText("Misc");

		components::command_button<"taxi">();

		components::command_button<"gunvan">();
	}
}
