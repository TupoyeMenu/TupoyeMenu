#include "views/view.hpp"

namespace big
{
	void view::waypoint_and_objective()
	{
		components::command_checkbox<"waypointbeacon">();

		if (g.world.waypoint_n_objective.waypoint_beacon)
			ImGui::ColorPicker3("Waypoint Color", g.world.waypoint_n_objective.waypoint_beacon_color, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);

		components::command_checkbox<"objectivebeacon">();

		if (g.world.waypoint_n_objective.objective_beacon)
			ImGui::ColorPicker3("Objective Color", g.world.waypoint_n_objective.objective_beacon_color, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
	}
}