#include "core/data/apartment_names.hpp"
#include "core/data/command_access_levels.hpp"
#include "core/data/region_codes.hpp"
#include "core/data/session_types.hpp"
#include "core/data/warehouse_names.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "hooking/hooking.hpp"
#include "util/chat.hpp"
#include "util/scripts.hpp"
#include "util/session.hpp"
#include "util/troll.hpp"
#include "util/toxic.hpp"
#include "views/view.hpp"

#include <network/Network.hpp>
#include <script/globals/GPBD_FM_3.hpp>
#include <script/globals/GlobalPlayerBD.hpp>

namespace big
{
	void view::network()
	{
		ImGui::TextUnformatted("Session Joiner");
		static uint64_t rid = 0;
		ImGui::InputScalar("Input RID", ImGuiDataType_U64, &rid);
		components::button("Join by RID", [] {
			session::join_by_rockstar_id(rid);
		});

		static char base64[500]{};
		components::input_text("Session Info", base64, sizeof(base64));
		components::button("Join Session Info", [] {
			rage::rlSessionInfo info;
			if (g_pointers->m_gta.m_decode_session_info(&info, base64, nullptr))
				session::join_session(info);
			else
				g_notification_service.push_error("Join", "Session info is invalid");
		});
		ImGui::SameLine();
		components::button("Copy Session Info", [] {
			char buf[0x100]{};
			g_pointers->m_gta.m_encode_session_info(&gta_util::get_network()->m_last_joined_session.m_session_info, buf, 0xA9, nullptr);
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
		ImGui::Checkbox("Fast Join", &g.session.fast_join);
		ImGui::SameLine();
		components::help_marker("This WILL break jobs");



		ImGui::Checkbox("Don't unload online maps", &g.tunables.dont_unload_online_maps);
		ImGui::SameLine();
		components::help_marker("Prevents ON_ENTER_SP from being called.\nSpeeds up joining online after going into single player.");

		ImGui::Checkbox("Join in SCTV slots", &g.session.join_in_sctv_slots);
		ImGui::SameLine();
		components::help_marker("Allows you to join full and solo sessions but can be detected by other modders");

		ImGui::BeginDisabled(!g_player_service->get_self()->is_host());

		ImGui::Checkbox("Lobby Lock", &g.session.lock_session);
		ImGui::SameLine();
		components::help_marker("Blocks all players from joining. May not work on some modders.");

		ImGui::Checkbox("Allow Friends Into Locked Lobby", &g.session.allow_friends_into_locked_session);
		ImGui::SameLine();
		components::help_marker("Allows Friends to Join Lobby While Locked");

		ImGui::EndDisabled();

		ImGui::SeparatorText("Player Magnet");
		ImGui::Checkbox("Enabled", &g.session.player_magnet_enabled);

		if (g.session.player_magnet_enabled)
		{
			ImGui::InputInt("Player Count", &g.session.player_magnet_count);
		}

		ImGui::SeparatorText("Chat");
		ImGui::Checkbox("Use Spam Timer", &g.session.use_spam_timer);
		ImGui::BeginDisabled(!g.session.use_spam_timer);
		ImGui::SliderFloat("Spam Timer", &g.session.spam_timer, 0.5f, 5.0f);
		ImGui::SliderInt("Spam Text Length", &g.session.spam_length, 1, 256);
		ImGui::EndDisabled();
		ImGui::Checkbox("Log Chat Messages", &g.session.log_chat_messages);
		static char msg[256];
		components::input_text("##message", msg, sizeof(msg));
		ImGui::SameLine();
		ImGui::Checkbox("Is Team", &g.chat.local);
		ImGui::SameLine();
		components::button("Send", [] {
			if (const auto net_game_player = gta_util::get_network_player_mgr()->m_local_net_player; net_game_player)
			{
				chat::send_message(msg, nullptr, true, g.session.is_team);
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
		components::script_patch_checkbox("Reveal Hidden Players", &g.session.unhide_players_from_player_list, "Reveals players that have hidden themselves from the player list");

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
					scripts::force_host("freemode"_J);
					if (auto script = gta_util::find_script_thread("freemode"_J); script && script->m_net_component)
						((CGameScriptHandlerNetComponent*)script->m_net_component)->block_host_migration(true);

					scripts::force_host("fmmc_launcher"_J);
					if (auto script = gta_util::find_script_thread("fmmc_launcher"_J); script && script->m_net_component)
						((CGameScriptHandlerNetComponent*)script->m_net_component)->block_host_migration(true);
				});
		}
		ImGui::SameLine();
		components::help_marker("This might break freemode missions and interiors. Use with caution");

		ImGui::SeparatorText("All Players");
		ImGui::Checkbox("Off The Radar", &g.session.off_radar_all);
		ImGui::SameLine();
		ImGui::Checkbox("Never Wanted", &g.session.never_wanted_all);
		ImGui::SameLine();
		ImGui::Checkbox("Semi Godmode", &g.session.semi_godmode_all);

		ImGui::Checkbox("Explosion Karma", &g.session.explosion_karma);
		ImGui::SameLine();
		ImGui::Checkbox("Damage Karma", &g.session.damage_karma);
		ImGui::SameLine();
		ImGui::Checkbox("Fix Vehicle", &g.session.vehicle_fix_all);

		components::command_checkbox<"harass">();
		ImGui::SameLine();
		ImGui::Checkbox("Spam Killfeed", &g.session.spam_killfeed);

		bool_command whitelist_friends("trustfriends",
		    "Trust friends",
		    "Friends won't be flagged as modders or taken actions by reactions",
		    g.session.trust_friends);
		bool_command whitelist_session("trustsession",
		    "Trust this session",
		    "Players in this session won't be flagged as modders or taken actions by reactions",
		    g.session.trust_session);

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

			ImGui::SeparatorText("Bounty");
			static int value = 10000;
			ImGui::SliderInt("##bountyvalue", &value, 0, 10000);
			components::command_checkbox<"anonbounty">();
			components::button("Bounty All", [] {
				g_player_service->iterate([](auto& plyr) {
					troll::set_bounty_on_player(plyr.second, value, g.session.anonymous_bounty);
				});
			});

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
		components::command_button<"emptysession">();

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

		components::script_patch_checkbox("Block CEO Raids", &g.session.block_ceo_raids, "For the entire session");
		ImGui::SameLine();
		components::command_checkbox<"blockceos">();

		ImGui::EndGroup();
	}
}
