#include "views/view.hpp"
#include "fiber_pool.hpp"
#include "util/globals.hpp"
#include "util/mobile.hpp"
#include "util/teleport.hpp"
#include "util/vehicle.hpp"

namespace big
{
	void view::teleport()
	{
		ImGui::Text("Blips:");

		components::command_button<"waypointtp">({}, "Waypoint");
		ImGui::SameLine();
		components::command_button<"objectivetp">({}, "Objective");
		components::command_checkbox<"autotptowp">();

		ImGui::Text("Vehicles:");

		components::command_button<"lastvehtp">();
		ImGui::SameLine();
		components::command_button<"bringpv">();
		ImGui::SameLine();
		components::command_button<"pvtp">();

		ImGui::Text("Manual:");
		
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
