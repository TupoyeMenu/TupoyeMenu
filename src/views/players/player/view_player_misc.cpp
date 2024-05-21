#include "gta_util.hpp"
#include "util/chat.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_misc()
	{
		std::string title = std::format("Player Misc Options: {}", g_player_service->get_selected()->get_name());
		ImGui::TextUnformatted(title.c_str());

		components::player_command_button<"joinceo">(g_player_service->get_selected());
		components::player_command_button<"enterint">(g_player_service->get_selected());
		components::player_command_button<"copyoutfit">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"copymodel">(g_player_service->get_selected());

		components::player_command_button<"clearwanted">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"givehealth">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"givearmor">(g_player_service->get_selected());
		ImGui::SameLine();
		components::player_command_button<"giveammo">(g_player_service->get_selected());

		ImGui::Checkbox("Off The Radar", &g_player_service->get_selected()->off_radar);
		ImGui::Checkbox("Never Wanted", &g_player_service->get_selected()->never_wanted);
		ImGui::Checkbox("Semi Godmode", &g_player_service->get_selected()->semi_godmode);
		ImGui::Checkbox("Fix Vehicle", &g_player_service->get_selected()->fix_vehicle);

		ImGui::SeparatorText("Chat");

		ImGui::TextUnformatted("Send chat message to this player only.");

		static char msg[256];
		components::input_text("##message", msg, sizeof(msg));
		ImGui::SameLine();
		components::button("Send", [] {
			if (const auto net_game_player = gta_util::get_network_player_mgr()->m_local_net_player; net_game_player)
			{
				chat::send_message(msg, g_player_service->get_selected(), true, true);
			}
		});


		ImGui::SeparatorText("Debug");

		ImGui::Checkbox("Block Explosions", &g_player_service->get_selected()->block_explosions);
		ImGui::Checkbox("Block Clone Creates", &g_player_service->get_selected()->block_clone_create);
		ImGui::Checkbox("Block Clone Syncs", &g_player_service->get_selected()->block_clone_sync);
		ImGui::Checkbox("Block Network Events", &g_player_service->get_selected()->block_net_events);
		ImGui::Checkbox("Block PTFX", &g_player_service->get_selected()->block_ptfx);
		ImGui::Checkbox("Log Clones", &g_player_service->get_selected()->log_clones);
	}
}