/**
 * @file view_player_toxic.cpp
 * @brief Toxic options to disrupt the game for selected player.
 */

#include "core/data/apartment_names.hpp"
#include "core/data/warehouse_names.hpp"
#include "util/ped.hpp"
#include "util/teleport.hpp"
#include "util/toxic.hpp"
#include "util/vehicle.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_toxic()
	{
		std::string title = std::format("Player Toxic Options: {}", g_player_service->get_selected()->get_name());

		ImGui::TextUnformatted(title.c_str());

		if (ImGui::TreeNode("Ped"))
		{
			components::player_command_button<"kill">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"explode">(g_player_service->get_selected(), {});
			components::player_command_button<"ragdoll">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"beast">(g_player_service->get_selected(), {});
			ImGui::TreePop();
		}

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

			components::player_command_button<"apartmenttp">(g_player_service->get_selected(), {(uint64_t)g.session.send_to_apartment_idx});

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

			components::player_command_button<"warehousetp">(g_player_service->get_selected(), {(uint64_t)g.session.send_to_warehouse_idx});

			components::button("TP To Darts", [] {
				toxic::start_activity(g_player_service->get_selected(), eActivityType::Darts);
			});
			ImGui::SameLine();
			components::button("TP To Flight School", [] {
				toxic::start_activity(g_player_service->get_selected(), eActivityType::PilotSchool);
			});
			ImGui::SameLine();
			components::button("TP To Map Center", [] {
				toxic::start_activity(g_player_service->get_selected(), eActivityType::ArmWresling);
			});
			components::button("TP To Skydive", [] {
				toxic::start_activity(g_player_service->get_selected(), eActivityType::Skydive);
			});
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {81}, "TP To MOC");

			components::player_command_button<"interiortp">(g_player_service->get_selected(), {123}, "TP To Casino");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {124}, "TP To Penthouse");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {128}, "TP To Arcade");

			components::player_command_button<"interiortp">(g_player_service->get_selected(), {146}, "TP To Music Locker");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {148}, "TP To Record A Studios");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {149}, "TP To Custom Auto Shop");

			components::player_command_button<"interiortp">(g_player_service->get_selected(), {155}, "TP To Agency");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {160}, "TP To Freakshop");
			ImGui::SameLine();
			components::player_command_button<"interiortp">(g_player_service->get_selected(), {161}, "TP To Multi Floor Garage");

			if (g_player_service->get_selected()->get_ped())
			{
				static float new_location[3];
				auto& current_location = *reinterpret_cast<float(*)[3]>(g_player_service->get_selected()->get_ped()->get_position());

				components::small_text("Custom TP");
				ImGui::SetNextItemWidth(400);
				ImGui::InputFloat3("##customlocation", new_location);
				components::button("TP", [] {
					teleport::teleport_player_to_coords(g_player_service->get_selected(), *reinterpret_cast<rage::fvector3*>(&new_location));
				});
				ImGui::SameLine();
				if (ImGui::Button("Get current"))
				{
					std::copy(std::begin(current_location), std::end(current_location), std::begin(new_location));
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spawns"))
		{
			components::player_command_button<"spawn_grief_jesus">(g_player_service->get_selected(), {});
			components::player_command_button<"spawn_extr_grief_jesus">(g_player_service->get_selected(), {});
			components::player_command_button<"spawn_grief_lazer">(g_player_service->get_selected(), {});

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Warp Time (requires session host)"))
		{
			components::button("+1 Minute", [] {
				toxic::warp_time_forward(g_player_service->get_selected(), 60 * 1000);
			});
			ImGui::SameLine();
			components::button("+5 Minutes", [] {
				toxic::warp_time_forward(g_player_service->get_selected(), 5 * 60 * 1000);
			});
			ImGui::SameLine();
			components::button("+48 Minutes", [] {
				toxic::warp_time_forward(g_player_service->get_selected(), 48 * 60 * 1000);
			});
			ImGui::SameLine();
			components::button("+96 Minutes", [] {
				toxic::warp_time_forward(g_player_service->get_selected(), 96 * 60 * 1000);
			});
			ImGui::SameLine();
			components::button("+200 Minutes", [] {
				toxic::warp_time_forward(g_player_service->get_selected(), 200 * 60 * 1000);
			});
			ImGui::SameLine();
			components::button("Stop Time", [] {
				toxic::set_time(g_player_service->get_selected(), INT_MAX - 3000);
			});
			ImGui::SameLine();
			components::help_marker("This cannot be reversed. Use with caution");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Loops"))
		{
			ImGui::Checkbox("Kill Loop", &g_player_service->get_selected()->kill_loop);
			ImGui::SameLine();
			ImGui::Checkbox("Explosion Loop", &g_player_service->get_selected()->explosion_loop);

			ImGui::Checkbox("Ragdoll Loop", &g_player_service->get_selected()->ragdoll_loop);
			ImGui::SameLine();
			ImGui::Checkbox("Rotate Cam Loop", &g_player_service->get_selected()->rotate_cam_loop);
			ImGui::SameLine();
			components::help_marker("Also brings the player out of godmode if the event isn't blocked");

			ImGui::Checkbox("Spam Killfeed", &g_player_service->get_selected()->spam_killfeed);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Weapons"))
		{
			components::player_command_button<"giveweaps">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"remweaps">(g_player_service->get_selected(), {});

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Misc"))
		{
			components::player_command_button<"ceokick">(g_player_service->get_selected(), {});
			components::player_command_button<"intkick">(g_player_service->get_selected(), {});

			components::player_command_button<"tutorial">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"remweaps">(g_player_service->get_selected(), {});

			components::player_command_button<"golf">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"flightschool">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"darts">(g_player_service->get_selected(), {});

			components::player_command_button<"badlands">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"spacemonkey">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"wizard">(g_player_service->get_selected(), {});

			components::player_command_button<"qub3d">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"camhedz">(g_player_service->get_selected(), {});

			components::player_command_button<"mission">(g_player_service->get_selected(), {});

			components::player_command_button<"ceoraid">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::button("Trigger MC Raid", [] {
				toxic::start_activity(g_player_service->get_selected(), eActivityType::BikerDefend);
			});
			ImGui::SameLine();
			components::button("Trigger Bunker Raid", [] {
				toxic::start_activity(g_player_service->get_selected(), eActivityType::GunrunningDefend);
			});

			components::player_command_button<"sext">(g_player_service->get_selected(), {});
			ImGui::SameLine();
			components::player_command_button<"fakeban">(g_player_service->get_selected(), {});

			static int wanted_level;
			components::small_text("Wanted Level");
			ImGui::SliderInt("##wantedlevelslider", &wanted_level, 0, 5);
			components::player_command_button<"wanted">(g_player_service->get_selected(), {(uint64_t)wanted_level}, "Set");

			ImGui::TreePop();
		}
		ImGui::EndGroup();
	}
}