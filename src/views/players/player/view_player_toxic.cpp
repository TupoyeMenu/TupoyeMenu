#include "views/view.hpp"
#include "util/teleport.hpp"
#include "util/toxic.hpp"
#include "util/ped.hpp"
#include "util/vehicle.hpp"
#include "core/data/apartment_names.hpp"
#include "core/data/warehouse_names.hpp"

namespace big
{
	void view::view_player_toxic()
	{
		std::string title = std::format("Player Toxic Options: {}", g_player_service->get_selected()->get_name());

		ImGui::Text(title.c_str());

		components::player_command_button<"kill">(g_player_service->get_selected(), {});
		ImGui::SameLine();
		components::player_command_button<"explode">(g_player_service->get_selected(), {});
		components::player_command_button<"ceokick">(g_player_service->get_selected(), {});		
		components::player_command_button<"ragdoll">(g_player_service->get_selected(), {});
		components::player_command_button<"intkick">(g_player_service->get_selected(), {});
		components::player_command_button<"beast">(g_player_service->get_selected(), {});

		static int wanted_level;
		ImGui::SliderInt("Wanted Level", &wanted_level, 0, 5);
		ImGui::SameLine();
		components::player_command_button<"wanted">(g_player_service->get_selected(), { (uint64_t)wanted_level }, "Set");

		if (ImGui::TreeNode("Teleports"))
		{
			if (ImGui::BeginCombo("##apartment", apartment_names[g.session.send_to_apartment_idx]))
			{
				for (int i = 1; i < apartment_names.size(); i++)
				{
					if (ImGui::Selectable(apartment_names[i], i == g.session.send_to_apartment_idx))
					{
						g.session.send_to_apartment_idx = i;
					}
					if (i == g.session.send_to_apartment_idx)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();

			components::player_command_button<"apartmenttp">(g_player_service->get_selected(), { (uint64_t)g.session.send_to_apartment_idx });

			if (ImGui::BeginCombo("##warehouse", warehouse_names[g.session.send_to_warehouse_idx]))
			{
				for (int i = 1; i < warehouse_names.size(); i++)
				{
					if (ImGui::Selectable(warehouse_names[i], i == g.session.send_to_warehouse_idx))
					{
						g.session.send_to_warehouse_idx = i;
					}
					if (i == g.session.send_to_warehouse_idx)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();

			components::player_command_button<"warehousetp">(g_player_service->get_selected(), { (uint64_t)g.session.send_to_warehouse_idx });

			components::button("TP To Darts", [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::Darts); });
			ImGui::SameLine();
			components::button("TP To Flight School", [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::PilotSchool); });
			ImGui::SameLine();
			components::button("TP To Map Center", [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::ArmWresling); });
			components::button("TP To Skydive", [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::Skydive); });
			ImGui::SameLine();
			components::player_command_button<"cayotp">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 81 }, "TP To MOC");

			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 123 }, "TP To Casino");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 124 }, "TP To Penthouse");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 128 }, "TP To Arcade");

			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 146 }, "TP To Music Locker");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 148 }, "TP To Record A Studios");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 149 }, "TP To Custom Auto Shop");

			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 155 }, "TP To Agency");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 160 }, "TP To Freakshop");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), { 161 }, "TP To Multi Floor Garage");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Vehicle")) 
		{
			components::player_command_button<"vehkick">(g_player_service->get_selected(), {});
			components::player_command_button<"fullacceleration">(g_player_service->get_selected(), {});
			components::player_command_button<"launchvehicleup">(g_player_service->get_selected(), {});

			components::button("Repair Vehicle", [] {
				Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(g_player_service->get_selected()->id(), false);
				vehicle::repair(veh);
			});
			ImGui::SameLine(); components::help_marker("Repairs players vehicle. \nRequires entity control. \nBlacked by most internal menus.");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spawns")) // TODO: Clean this up.
		{
			components::button("Spawn Griefer Jesus", [] {
				Ped player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->get_selected()->id());
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(player_ped, true) + Vector3(0, 0, 10);
				ped::spawn_griefer_jesus(pos, player_ped);
			});
			ImGui::SameLine(); components::help_marker("Spawns 'Griefer Jesus' just like in ChaosMod.");
			ImGui::SameLine();

			components::button("Spawn Extreme Griefer Jesus", [] {
				Ped player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->get_selected()->id());
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(player_ped, true) + Vector3(0, 0, 10);

				ped::spawn_extrime_griefer_jesus(pos, player_ped);
			});
			ImGui::SameLine(); components::help_marker("Spawns 'Extreme Griefer Jesus' just like in ChaosMod.");

			components::button("Spawn Griefer Lazer", [] {
				Ped player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->get_selected()->id());
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(player_ped, true) + Vector3(0, 0, 500);
				Hash jet_hash RAGE_JOAAT("lazer");
				ped::spawn_griefer_jet(pos, player_ped, jet_hash);
			});
			ImGui::SameLine(); components::help_marker("Spawns a lazer with Griefer Jesus in it.");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Warp Time (requires session host)"))
		{
			components::button("+1 Minute", [] { toxic::warp_time_forward(g_player_service->get_selected(), 60 * 1000); });
			ImGui::SameLine();
			components::button("+5 Minutes", [] { toxic::warp_time_forward(g_player_service->get_selected(), 5 * 60 * 1000); });
			ImGui::SameLine();
			components::button("+48 Minutes", [] { toxic::warp_time_forward(g_player_service->get_selected(), 48 * 60 * 1000); });
			ImGui::SameLine();
			components::button("+96 Minutes", [] { toxic::warp_time_forward(g_player_service->get_selected(), 96 * 60 * 1000); });
			ImGui::SameLine();
			components::button("+200 Minutes", [] { toxic::warp_time_forward(g_player_service->get_selected(), 200 * 60 * 1000); });
			ImGui::SameLine();
			components::button("Stop Time", [] { toxic::set_time(g_player_service->get_selected(), INT_MAX - 3000); });
			ImGui::SameLine(); components::help_marker("This cannot be reversed. Use with caution");

			ImGui::Checkbox("Kill Loop", &g_player_service->get_selected()->kill_loop);
			ImGui::SameLine();
			ImGui::Checkbox("Explosion Loop", &g_player_service->get_selected()->explosion_loop);
			ImGui::SameLine();
			ImGui::Checkbox("Freeze Loop", &g_player_service->get_selected()->freeze_loop);

			ImGui::Checkbox("Ragdoll Loop", &g_player_service->get_selected()->ragdoll_loop);
			ImGui::SameLine();
			ImGui::Checkbox("Rotate Cam Loop", &g_player_service->get_selected()->rotate_cam_loop);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Also brings the player out of godmode if the event isn't blocked");

			ImGui::TreePop();
		}

		components::player_command_button<"giveweaps">(g_player_service->get_selected(), { });
		ImGui::SameLine();
		components::player_command_button<"remweaps">(g_player_service->get_selected(), { });
	}
}