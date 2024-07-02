/**
 * @file view_player_kicks_and_crashes.cpp
 * @brief Options to kick or crash selected player.
 * @note Define `ENABLE_CRASHES` to enable crashes.
 * @todo Fully remove crashes.
 */

#include "gta_util.hpp"
#include "views/view.hpp"

#include <network/Network.hpp>

namespace big
{
	void view::view_player_kicks_and_crashes()
	{
		ImGui::Text("Player Kick Options: %s", g_player_service->get_selected()->get_name());
		auto const is_session_host = [] {
			return gta_util::get_network()->m_game_session_ptr->is_host();
		};

		if (!g_player_service->get_self()->is_host())
			ImGui::TextUnformatted("Host and breakup kick require session host");

		ImGui::BeginDisabled(!g_player_service->get_self()->is_host());

		components::player_command_button<"hostkick">(g_player_service->get_selected());
		components::player_command_button<"breakup">(g_player_service->get_selected());
		ImGui::SameLine();
		components::command_checkbox<"breakupcheating">();

		ImGui::EndDisabled();

		components::player_command_button<"smartkick">(g_player_service->get_selected());

		components::player_command_button<"oomkick">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"shkick">(g_player_service->get_selected());

		components::player_command_button<"endkick">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"desync">(g_player_service->get_selected());

#ifdef ENABLE_CRASHES
		ImGui::SeparatorText("Crashes (shit)");
		components::player_command_button<"invalidmodelcrash">(g_player_service->get_selected());
		components::player_command_button<"tsecrash">(g_player_service->get_selected());
#endif // ENABLE_CRASHES
	}
}
