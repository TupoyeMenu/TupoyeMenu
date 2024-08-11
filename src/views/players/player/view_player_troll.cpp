#include "util/teleport.hpp"
#include "util/troll.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_troll()
	{
		ImGui::Text("Player Troll Options: %s", g_player_service->get_selected()->get_name());

		components::player_command_button<"playertp">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"bring">(g_player_service->get_selected());
		ImGui::SameLine();
		components::button("TP To Waypoint", [] {
			Vector3 location;
			if (blip::get_blip_location(location, (int)BlipIcons::RADAR_WAYPOINT))
				entity::load_ground_at_3dcoord(location), teleport::teleport_player_to_coords(g_player_service->get_selected(), location);
		});

		components::player_command_button<"playervehtp">(g_player_service->get_selected());
		components::player_command_button<"rcplayer">(g_player_service->get_selected());

		components::button("Godmode kill", [] {
			teleport::godmode_kill(g_player_service->get_selected());
		});
		ImGui::SameLine();
		components::help_marker("Teleports players vehicle into killbox. \nBlocked by most internal menus.");

		static int bounty_value = 0;

		ImGui::SliderInt("Bounty", &bounty_value, 0, 10000);
		ImGui::SameLine();
		components::command_checkbox<"anonbounty">();
		ImGui::SameLine();
		ImGui::PushID("setbounty");
		components::button("Set", [] {
			troll::set_bounty_on_player(g_player_service->get_selected(), bounty_value, g.session.anonymous_bounty);
		});
		ImGui::PopID();
	}
}