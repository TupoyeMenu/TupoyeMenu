/**
 * @file view_session.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/apartment_names.hpp"
#include "core/data/command_access_levels.hpp"
#include "core/data/region_codes.hpp"
#include "core/data/warehouse_names.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "hooking.hpp"
#include "util/notify.hpp"
#include "util/scripts.hpp"
#include "util/session.hpp"
#include "util/toxic.hpp"
#include "views/view.hpp"

#include <network/Network.hpp>
#include <script/globals/GPBD_FM_3.hpp>
#include <script/globals/GlobalPlayerBD.hpp>

namespace big
{
	void view::session()
	{
		ImGui::Text("Session Joiner");
		static uint64_t rid = 0;
		ImGui::InputScalar("Input RID", ImGuiDataType_U64, &rid);
		components::button("Join by RID", [] {
			session::join_by_rockstar_id(rid);
		});

		static char base64[500]{};
		components::input_text("Session Info", base64, sizeof(base64));
		components::button("Join Session Info", [] {
			rage::rlSessionInfo info;
			g_pointers->m_gta.m_decode_session_info(&info, base64, nullptr);
			session::join_session(info);
		});
		ImGui::SameLine();
		components::button("Copy Session Info", [] {
			char buf[0x100]{};
			g_pointers->m_gta.m_encode_session_info(&gta_util::get_network()->m_game_session.m_rline_session.m_session_info, buf, 0xA9, nullptr);
			ImGui::SetClipboardText(buf);
		});

		if (ImGui::TreeNode("Session Switcher"))
		{
			if (ImGui::BeginListBox("###session_switch"))
			{
				for (const auto& session_type : sessions)
				{
					components::selectable(session_type.name, false, [&session_type] {
						session::join_type(session_type.id);
					});
				}
				ImGui::EndListBox();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Region Switcher"))
		{
			if (ImGui::BeginListBox("###region_switch"))
			{
				for (const auto& region_type : regions)
				{
					components::selectable(region_type.name, *g_pointers->m_gta.m_region_code == region_type.id, [&region_type] {
						*g_pointers->m_gta.m_region_code = region_type.id;
					});
				}
				ImGui::EndListBox();
			}

			ImGui::TreePop();
		}

		ImGui::SeparatorText("Join options");

		ImGui::Checkbox("Seamless Join", &g.tunables.seamless_join);
		ImGui::SameLine();
		components::help_marker("Allows you move freely while in session transition.");

		ImGui::Checkbox("Don't unload online maps", &g.tunables.dont_unload_online_maps);
		ImGui::SameLine();
		components::help_marker("Prevents ON_ENTER_SP from being called.\nSpeeds up joining online after going into single player.");

		ImGui::Checkbox("Join in SCTV slots", &g.session.join_in_sctv_slots);
		ImGui::SameLine();
		components::help_marker("Allows you to join full and solo sessions but can be detected by other modders");

		ImGui::SeparatorText("Player Magnet");
		ImGui::Checkbox("Enabled", &g.session.player_magnet_enabled);
		if (g.session.player_magnet_enabled)
		{
			ImGui::InputInt("Player Count", &g.session.player_magnet_count);
		}
		ImGui::SeparatorText("Chat");
		ImGui::Checkbox("Auto-kick Chat Spammers", &g.session.kick_chat_spammers);
		ImGui::Checkbox("Disable Filter", &g.session.chat_force_clean);
		ImGui::SameLine();
		components::help_marker("Your sent chat messages will not be censored to the receivers");
		ImGui::Checkbox("Log Chat Messages", &g.session.log_chat_messages);
		static char msg[256];
		components::input_text("##message", msg, sizeof(msg));
		ImGui::SameLine();
		ImGui::Checkbox("Is Team", &g.chat.local);
		ImGui::SameLine();
		components::button("Send", [] {
			if (const auto net_game_player = gta_util::get_network_player_mgr()->m_local_net_player; net_game_player)
			{
				if (g_hooking->get_original<hooks::send_chat_message>()(*g_pointers->m_gta.m_send_chat_ptr,
				        net_game_player->get_net_data(),
				        msg,
				        g.chat.local))
					notify::draw_chat(msg, net_game_player->get_name(), g.chat.local);
			}
		});

		ImGui::Checkbox("Chat Commands", &g.session.chat_commands);
		if (g.session.chat_commands)
		{
			if (ImGui::BeginCombo("Default Command Permissions", COMMAND_ACCESS_LEVELS[g.session.chat_command_default_access_level]))
			{
				for (const auto& [type, name] : COMMAND_ACCESS_LEVELS)
				{
					if (ImGui::Selectable(name, type == g.session.chat_command_default_access_level))
					{
						g.session.chat_command_default_access_level = type;
					}

					if (type == g.session.chat_command_default_access_level)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}
		}

		ImGui::SeparatorText("Decloak");
		components::script_patch_checkbox("Reveal OTR Players", &g.session.decloak_players);

		ImGui::SeparatorText("Force Host");
		ImGui::Checkbox("Force Session Host", &g.session.force_session_host);
		ImGui::SameLine();
		components::help_marker("Join another session to apply changes. The original host of the session must leave or be kicked. This feature is easily detectable by other mod menus, use with caution");

		ImGui::SameLine();
		if (g.session.force_session_host)
		{
			ImGui::SameLine();
			ImGui::Checkbox("Kick Host During Join", &g.session.kick_host_when_forcing_host);
		}

		if (ImGui::Checkbox("Force Script Host", &g.session.force_script_host))
		{
			if (g.session.force_script_host)
				g_fiber_pool->queue_job([] {
					scripts::force_host(RAGE_JOAAT("freemode"));
					if (auto script = gta_util::find_script_thread(RAGE_JOAAT("freemode")); script && script->m_net_component)
						script->m_net_component->block_host_migration(true);

					scripts::force_host(RAGE_JOAAT("fmmc_launcher"));
					if (auto script = gta_util::find_script_thread(RAGE_JOAAT("fmmc_launcher")); script && script->m_net_component)
						script->m_net_component->block_host_migration(true);
				});
		}

		ImGui::SeparatorText("All Players");
		ImGui::Checkbox("Off The Radar", &g.session.off_radar_all);
		ImGui::SameLine();
		ImGui::Checkbox("Never Wanted", &g.session.never_wanted_all);
		ImGui::SameLine();
		ImGui::Checkbox("Semi Godmode", &g.session.semi_godmode_all);

		ImGui::Checkbox("Explosion Karma", &g.session.explosion_karma);
		ImGui::SameLine();
		ImGui::Checkbox("Damage Karma", &g.session.damage_karma);

		static int global_wanted_level = 0;

		if (ImGui::SliderInt("Wanted Level", &global_wanted_level, 0, 5))
		{
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].RemoteWantedLevelAmount = global_wanted_level;
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Force", &g.session.wanted_level_all))
		{
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].RemoteWantedLevelPlayer = __rdtsc() + 32;
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].RemoteWantedLevelAmount = global_wanted_level;
		}

		if (ImGui::TreeNode("Toxic"))
		{
			components::command_button<"killall">({}, "Kill Everyone");
			ImGui::SameLine();
			components::command_button<"explodeall">({}, "Explode Everyone");

			ImGui::SameLine();

			components::command_button<"beastall">({});

			components::command_button<"giveweapsall">({});
			ImGui::SameLine();
			components::command_button<"remweapsall">({});

			components::command_button<"ceokickall">({});
			ImGui::SameLine();
			components::command_button<"vehkickall">({});


			components::command_button<"ragdollall">({}, "Ragdoll Players");
			ImGui::SameLine();
			components::command_button<"intkickall">({}, "Kick Everyone From Interiors");

			components::command_button<"missionall">({});

			components::command_button<"ceoraidall">({});
			ImGui::SameLine();
			components::button("Trigger MC Raid", [] {
				g_player_service->iterate([](auto& plyr) {
					toxic::start_activity(plyr.second, eActivityType::BikerDefend);
				});
			});
			ImGui::SameLine();
			components::button("Trigger Bunker Raid", [] {
				g_player_service->iterate([](auto& plyr) {
					toxic::start_activity(plyr.second, eActivityType::GunrunningDefend);
				});
			});

			components::command_button<"sextall">({}, "Send Sexts");
			ImGui::SameLine();
			components::command_button<"fakebanall">({}, "Send Fake Ban Messages");

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

			components::command_button<"apartmenttpall">({(uint64_t)g.session.send_to_apartment_idx}, "TP All To Apartment");

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

			components::command_button<"warehousetpall">({(uint64_t)g.session.send_to_warehouse_idx}, "TP All To Warehouse");

			components::button("TP All To Darts", [] {
				g_player_service->iterate([](auto& plyr) {
					toxic::start_activity(plyr.second, eActivityType::Darts);
				});
			});
			ImGui::SameLine();
			components::button("TP All To Flight School", [] {
				g_player_service->iterate([](auto& plyr) {
					toxic::start_activity(plyr.second, eActivityType::PilotSchool);
				});
			});
			ImGui::SameLine();
			components::button("TP All To Map Center", [] {
				g_player_service->iterate([](auto& plyr) {
					toxic::start_activity(plyr.second, eActivityType::ArmWresling);
				});
			});

			components::button("TP All To Skydive", [] {
				g_player_service->iterate([](auto& plyr) {
					toxic::start_activity(plyr.second, eActivityType::Skydive);
				});
			});
			ImGui::SameLine();

			components::command_button<"interiortpall">({81}, "TP All To MOC");

			ImGui::SameLine();
			components::command_button<"interiortpall">({123}, "TP All To Casino");
			ImGui::SameLine();
			components::command_button<"interiortpall">({124}, "TP All To Penthouse");
			ImGui::SameLine();
			components::command_button<"interiortpall">({128}, "TP All To Arcade");

			components::command_button<"interiortpall">({146}, "TP All To Music Locker");
			ImGui::SameLine();
			components::command_button<"interiortpall">({148}, "TP All To Record A Studios");
			ImGui::SameLine();
			components::command_button<"interiortpall">({149}, "TP All To Custom Auto Shop");

			components::command_button<"interiortpall">({155}, "TP All To Agency");
			ImGui::SameLine();
			components::command_button<"interiortpall">({160}, "TP All To Freakshop");
			ImGui::SameLine();
			components::command_button<"interiortpall">({161}, "TP All To Multi Floor Garage");

			ImGui::TreePop();
		}

		ImGui::Checkbox("Disable Pedestrians", &g.session.disable_peds);
		ImGui::SameLine();
		ImGui::Checkbox("Disable Traffic", &g.session.disable_traffic);
		ImGui::SameLine();
		ImGui::Checkbox("Force Thunder", &g.session.force_thunder);

		ImGui::SeparatorText("Warp Time (requires session host)");

		components::button("+1 Minute", [] {
			toxic::warp_time_forward_all(60 * 1000);
		});
		ImGui::SameLine();
		components::button("+5 Minutes", [] {
			toxic::warp_time_forward_all(5 * 60 * 1000);
		});
		ImGui::SameLine();
		components::button("+48 Minutes", [] {
			toxic::warp_time_forward_all(48 * 60 * 1000);
		});
		ImGui::SameLine();
		components::button("+96 Minutes", [] {
			toxic::warp_time_forward_all(96 * 60 * 1000);
		});
		ImGui::SameLine();
		components::button("+200 Minutes", [] {
			toxic::warp_time_forward_all(200 * 60 * 1000);
		});
		ImGui::SameLine();
		components::button("Stop Time", [] {
			toxic::set_time_all(INT_MAX - 3000);
		});
		ImGui::SameLine();
		components::help_marker("This cannot be reversed. Use with caution");

		ImGui::SeparatorText("Script Host Features");
		ImGui::Checkbox("Disable CEO Money", &g.session.block_ceo_money);
		ImGui::SameLine();
		components::help_marker("Blocks CEO money drops across the entire session. This can also break other stuff, use with caution");
		ImGui::SameLine();
		ImGui::Checkbox("Randomize CEO Colors", &g.session.randomize_ceo_colors);
		ImGui::Checkbox("Block Jobs", &g.session.block_jobs);
		ImGui::SameLine();
		components::help_marker("Prevents remote players from starting jobs while in your session");
		ImGui::SameLine();
		components::script_patch_checkbox("Block Muggers", &g.session.block_muggers, "For the entire session");
		ImGui::SameLine();
		components::script_patch_checkbox("Block CEO Raids", &g.session.block_ceo_raids, "For the entire session");
	}
}
