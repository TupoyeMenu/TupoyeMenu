#include "views/view.hpp"
#include "util/teleport.hpp"
#include "util/vehicle.hpp"
#include "util/troll.hpp"

namespace big
{
	void view::view_player_troll()
	{
		std::string title = std::format("Player Troll Options: {}", g_player_service->get_selected()->get_name());
		ImGui::Text(title.c_str());

		components::player_command_button<"playertp">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"bring">(g_player_service->get_selected());

		components::player_command_button<"playervehtp">(g_player_service->get_selected());
		components::player_command_button<"rcplayer">(g_player_service->get_selected());

		components::button("Godmode kill", [] { teleport::godmode_kill(g_player_service->get_selected()); });
		ImGui::SameLine(); components::help_marker("Teleports players vehicle into killbox. \nBlocked by most internal menus.");

		static int bounty_value = 0;
			
		ImGui::SliderInt("BOUNTY"_T.data(), &bounty_value, 0, 10000);
		ImGui::SameLine();
		components::command_checkbox<"anonbounty">();
		ImGui::SameLine();
		components::button("SET"_T, [] { troll::set_bounty_on_player(g_player_service->get_selected(), bounty_value, g.session.anonymous_bounty);});
	}
}