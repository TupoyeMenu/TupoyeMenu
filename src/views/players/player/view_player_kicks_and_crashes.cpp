#include "views/view.hpp"
#include "util/teleport.hpp"
#include "util/toxic.hpp"
#include "util/ped.hpp"
#include "util/vehicle.hpp"

namespace big
{
	void view::view_player_kicks_and_crashes()
	{
		std::string title = std::format("Player Kick and Crash Options: {}", g_player_service->get_selected()->get_name());
		ImGui::Text(title.c_str());

		if (ImGui::TreeNode("Kicks")) 
		{
			auto const is_session_host = [] { return gta_util::get_network()->m_game_session_ptr->is_host(); };

			components::player_command_button<"breakup">(g_player_service->get_selected());
			ImGui::SameLine();
			components::command_checkbox<"breakupcheating">();
			components::disable_unless(std::not_fn(is_session_host), []
			{
				components::player_command_button<"lckick">(g_player_service->get_selected());
			});
			components::player_command_button<"bailkick">(g_player_service->get_selected());
			components::player_command_button<"nfkick">(g_player_service->get_selected());
			components::player_command_button<"oomkick">(g_player_service->get_selected());
			components::player_command_button<"gikick">(g_player_service->get_selected());
			components::player_command_button<"shkick">(g_player_service->get_selected());
			components::player_command_button<"endkick">(g_player_service->get_selected());
			components::player_command_button<"desync">(g_player_service->get_selected());

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Crashes (shit)")) 
		{
			components::player_command_button<"slodpedcrash">(g_player_service->get_selected());
			components::player_command_button<"tlcrash">(g_player_service->get_selected());

			ImGui::TreePop();
		}
	}
}