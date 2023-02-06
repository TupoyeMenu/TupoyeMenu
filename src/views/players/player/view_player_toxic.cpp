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

		if (ImGui::TreeNode("PED"_T.data()))
		{
			components::player_command_button<"kill">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"explode">(g_player_service->get_selected(), {});
			components::player_command_button<"ragdoll">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"beast">(g_player_service->get_selected(), {});
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("TELEPORTS"_T.data()))
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

			components::button("TP_TO_DARTS"_T, [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::Darts); });
			ImGui::SameLine();
			components::button("TP_TO_FLIGHT_SCHOOL"_T, [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::PilotSchool); });
			ImGui::SameLine();
			components::button("TP_TO_MAP_CENTER"_T, [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::ArmWresling); });
			components::button("TP_TO_SKYDIVE"_T, [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::Skydive); });
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

		if (ImGui::TreeNode("VEHICLE"_T.data())) 
		{
			components::player_command_button<"vehkick">(g_player_service->get_selected(), {});
			components::player_command_button<"fullacceleration">(g_player_service->get_selected(), {});
			components::player_command_button<"launchvehicleup">(g_player_service->get_selected(), {});
			
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spawns"))
		{
			components::player_command_button<"spawn_grief_jesus">(g_player_service->get_selected(), {});
			components::player_command_button<"spawn_extr_grief_jesus">(g_player_service->get_selected(), {});
			components::player_command_button<"spawn_grief_lazer">(g_player_service->get_selected(), {});

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("WARP_TIME"_T.data()))
		{
			components::button("PLUS_1_MINUTE"_T.data(), [] { toxic::warp_time_forward(g_player_service->get_selected(), 60 * 1000); });
			ImGui::SameLine();
			components::button("PLUS_5_MINUTES"_T.data(), [] { toxic::warp_time_forward(g_player_service->get_selected(), 5 * 60 * 1000); });
			ImGui::SameLine();
			components::button("PLUS_48_MINUTES"_T.data(), [] { toxic::warp_time_forward(g_player_service->get_selected(), 48 * 60 * 1000); });
			ImGui::SameLine();
			components::button("PLUS_96_MINUTES"_T.data(), [] { toxic::warp_time_forward(g_player_service->get_selected(), 96 * 60 * 1000); });
			ImGui::SameLine();
			components::button("PLUS_200_MINUTES"_T.data(), [] { toxic::warp_time_forward(g_player_service->get_selected(), 200 * 60 * 1000); });
			ImGui::SameLine();
			components::button("STOP_TIME"_T.data(), [] { toxic::set_time(g_player_service->get_selected(), INT_MAX - 3000); });
			ImGui::SameLine(); components::help_marker("STOP_TIME_DESC"_T);

			ImGui::Checkbox("KILL_LOOP"_T.data(), &g_player_service->get_selected()->kill_loop);
			ImGui::SameLine();
			ImGui::Checkbox("EXPLOSION_LOOP"_T.data(), &g_player_service->get_selected()->explosion_loop);
			ImGui::SameLine();
			ImGui::Checkbox("FREEZE_LOOP"_T.data(), &g_player_service->get_selected()->freeze_loop);

			ImGui::Checkbox("RAGDOLL_LOOP"_T.data(), &g_player_service->get_selected()->ragdoll_loop);
			ImGui::SameLine();
			ImGui::Checkbox("ROT_CAM_LOOP"_T.data(), &g_player_service->get_selected()->rotate_cam_loop);
			ImGui::SameLine(); components::help_marker("PLAYER_TOXIC_BRING_PLAYER_OUT_GOD"_T);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Weapons"))
		{
			components::player_command_button<"giveweaps">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"remweaps">(g_player_service->get_selected(), { });

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("MISC"_T.data()))
		{
			components::player_command_button<"ceokick">(g_player_service->get_selected(), {});		
			components::player_command_button<"intkick">(g_player_service->get_selected(), {});

			components::player_command_button<"tutorial">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"golf">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"flightschool">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"darts">(g_player_service->get_selected(), { });

			components::player_command_button<"badlands">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"spacemonkey">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"wizard">(g_player_service->get_selected(), { });

			components::player_command_button<"qub3d">(g_player_service->get_selected(), { });
			ImGui::SameLine();
			components::player_command_button<"camhedz">(g_player_service->get_selected(), { });

			components::player_command_button<"mission">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"error">(g_player_service->get_selected(), {});

			components::player_command_button<"ceoraid">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::button("Trigger MC Raid", [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::BikerDefend); });
			ImGui::SameLine();
			components::button("Trigger Bunker Raid", [] { toxic::start_activity(g_player_service->get_selected(), eActivityType::GunrunningDefend); });

			components::player_command_button<"sext">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"fakeban">(g_player_service->get_selected(), {});

			static int wanted_level;
			ImGui::SliderInt("WANTED_LVL"_T.data(), &wanted_level, 0, 5);
			ImGui::SameLine();
			components::player_command_button<"wanted">(g_player_service->get_selected(), { (uint64_t)wanted_level }, "Set");

			ImGui::TreePop();
		}

		
	}
}