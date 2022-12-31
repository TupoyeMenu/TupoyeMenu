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
			components::player_command_button<"shkick">(g_player_service->get_selected());
			components::player_command_button<"endkick">(g_player_service->get_selected());
			components::disable_unless(std::function(is_session_host), []
			{
				components::player_command_button<"hostkick">(g_player_service->get_selected());
			});
			components::player_command_button<"desync">(g_player_service->get_selected());
		}

		if (ImGui::TreeNode("Crashes (shit)")) 
		{
			components::button("Ped Crash", [] {
				Ped ped = ped::spawn(ePedType::PED_TYPE_PROSTITUTE, rage::joaat("slod_human"), 0, misc::fvector3_to_Vector3(*g_player_service->get_selected()->get_ped()->get_position()), 0);
				script::get_current()->yield(3s);
				entity::delete_entity_notp(ped);
			});
			ImGui::SameLine(); components::help_marker("Spawns 'slod_human' ped near player wich crashes them. \nBlocked by most internal menus.\nUntested.");

			components::button("Vehicle Crash", [] {
				Vehicle veh = vehicle::spawn(rage::joaat("arbitergt"), misc::fvector3_to_Vector3(*g_player_service->get_selected()->get_ped()->get_position()), 0.f);
				script::get_current()->yield(3s);
				entity::delete_entity_notp(veh);
			});
			ImGui::SameLine(); components::help_marker("Spawns next_gen vehicle: 'arbitergt' near player wich crashes them. \nBlocked by most internal menus.\nUntested.");
			ImGui::TreePop();
		}
	}
}