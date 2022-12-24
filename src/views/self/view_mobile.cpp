#include "fiber_pool.hpp"
#include "services/mobile/mobile_service.hpp"
#include "util/mobile.hpp"
#include "views/view.hpp"
#include "core/data/phone_types.hpp"

namespace big
{
	void view::mobile() 
	{
		components::sub_title("Merryweather");

		ImGui::Separator();

		components::button("Request Ammo Drop", [] {
			mobile::merry_weather::request_ammo_drop();
		});

		components::button("Helicopter Pickup", [] {
			mobile::merry_weather::request_helicopter_pickup();
		});

		ImGui::Separator();

		components::button("Mors Mutual Fix All Vehicles", [] {
			int amount_fixed = mobile::mors_mutual::fix_all();
			g_notification_service->push("Mobile",
				std::format("{} vehicle{} been fixed.", amount_fixed, amount_fixed == 1 ? " has" : "s have")
			);
		});
	}
}
