#include "gta_util.hpp"
#include "util/chat.hpp"
#include "util/scripts.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_misc()
	{
		ImGui::Text("Player Misc Options: %s", g_player_service->get_selected()->get_name());

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
		ImGui::SameLine();
		components::player_command_button<"giveweaps">(g_player_service->get_selected(), {});

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

		ImGui::SeparatorText("b3258");

		components::button("Trigger UFO Abduction", [] {
			scripts::force_host("freemode"_J);
			g_player_service->get_selected()->script_host_mission = 9999;
			scripts::force_script_on_player(g_player_service->get_selected(), "fm_content_ufo_abduction"_J, g_player_service->get_selected()->id());
			for (int i = 0; i < 10; i++)
			{
				memset(scr_globals::gsbd_fm_events.at(11).at(143).as<void*>(), __rdtsc(), sizeof(std::uint64_t) * 4);
				script::get_current()->yield(30ms);
			}
		});

		// Broken
		components::button("Trigger Pizza Delivery", [] {
			scripts::force_host("freemode"_J);
			g_player_service->get_selected()->script_host_mission = 9999;
			scripts::force_script_on_player(g_player_service->get_selected(), "fm_content_pizza_delivery"_J);
			for (int i = 0; i < 10; i++)
			{
				memset(scr_globals::gsbd_fm_events.at(11).at(143).as<void*>(), __rdtsc(), sizeof(std::uint64_t) * 4);
				script::get_current()->yield(30ms);
			}
		});
	}
}