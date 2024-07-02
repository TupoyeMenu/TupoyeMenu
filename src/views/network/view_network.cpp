#include "core/data/apartment_names.hpp"
#include "core/data/region_codes.hpp"
#include "core/data/warehouse_names.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "gui/components/components.hpp"
#include "hooking/hooking.hpp"
#include "util/session.hpp"
#include "util/troll.hpp"
#include "util/toxic.hpp"
#include "views/view.hpp"
#include "backend/bool_command.hpp"

#include <network/Network.hpp>
#include <script/globals/GPBD_FM_3.hpp>
#include <script/globals/GlobalPlayerBD.hpp>


namespace big
{
	struct SessionType
	{
		eSessionType id;
		const char* name;
	};

	void render_join_game()
	{
		ImGui::SeparatorText("Join Game");

		ImGui::BeginGroup();
		static uint64_t rid = 0;
		static char username[20];
		static char base64[500]{};
		
		ImGui::SetNextItemWidth(200);
		ImGui::InputScalar("##inputrid", ImGuiDataType_U64, &rid);
		ImGui::SameLine();
		components::button("Join by RID", [] {
			session::join_by_rockstar_id(rid);
		});

		ImGui::SetNextItemWidth(200);
		components::input_text_with_hint("##sessioninfoinput", "Session Info", base64, sizeof(base64));
		ImGui::SameLine();
		components::button("Join Session Info", [] {
			rage::rlSessionInfo info;
			if (g_pointers->m_gta.m_decode_session_info(&info, base64, nullptr))
				session::join_session(info);
			else
				g_notification_service.push_error("Rid Joiner", "Session info is invalid");
		});

		components::button("Copy Session Info", [] {
			char buf[0x100]{};
			g_pointers->m_gta.m_encode_session_info(&gta_util::get_network()->m_last_joined_session.m_session_info, buf, 0xA9, nullptr);
			ImGui::SetClipboardText(buf);
		});

		ImGui::Checkbox("Join in SCTV slots", &g.session.join_in_sctv_slots);
		ImGui::SameLine();
		components::help_marker("Allows you to join full and solo sessions but can be detected by other modders");

		ImGui::Checkbox("Player Magnet", &g.session.player_magnet_enabled);
		if (g.session.player_magnet_enabled)
		{
			ImGui::Text("Player Count");
			ImGui::InputInt("##playercount", &g.session.player_magnet_count);
		}
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		std::string region_str = regions[*g_pointers->m_gta.m_region_code].name;

		ImGui::SetNextItemWidth(250);
		if (ImGui::BeginCombo("##regionswitcher", region_str.c_str()))
		{
			for (const auto& region_type : regions)
			{
				components::selectable(region_type.name, *g_pointers->m_gta.m_region_code == region_type.id, [&region_type] {
					*g_pointers->m_gta.m_region_code = region_type.id;
				});
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		components::help_marker("SESSION_SELECT_COMBO_DESC, This does not have a translation because YimMenu doesn't have it.");

		ImGui::Separator();

		static constexpr auto sessions = std::to_array<SessionType>({// This has to be here because if it's generated at compile time, the translations break for some reason.
		    {eSessionType::JOIN_PUBLIC, "Join Public Session"},
		    {eSessionType::NEW_PUBLIC, "New Public Session"},
		    {eSessionType::CLOSED_CREW, "Closed Crew Session"},
		    {eSessionType::CREW, "Crew Session"},
		    {eSessionType::CLOSED_FRIENDS, "Closed Friend Session"},
		    {eSessionType::FIND_FRIEND, "Find Friend Session"},
		    {eSessionType::SOLO, "Solo Session"},
		    {eSessionType::INVITE_ONLY, "Invite Only Session"},
		    {eSessionType::JOIN_CREW, "Join Crew Session"},
		    {eSessionType::SC_TV, "Social Club TV"},
		    {eSessionType::LEAVE_ONLINE, "Leave GTA Online"}});

		for (const auto& [id, name] : sessions)
		{
			if (id == eSessionType::LEAVE_ONLINE && !*g_pointers->m_gta.m_is_session_started) // Don't show a Leave Online option in single player (it actually sends us INTO online)
				continue;

			components::selectable(name, false, [&id] {
				session::join_type(id);
			});
		}

		ImGui::EndGroup();
	}

	bool_command whitelist_friends("trustfriends", "Trust friends", "Friends won't be flagged as modders or taken actions by reactions", g.session.trust_friends);
	bool_command whitelist_session("trustsession", "Trust this session", "Players in this session won't be flagged as modders or taken actions by reactions", g.session.trust_session);

	void render_general_options()
	{
		ImGui::SeparatorText("Player Toggles");

		ImGui::BeginGroup();
		components::command_checkbox<"trustfriends">();
		components::command_checkbox<"trustsession">();
		components::script_patch_checkbox("Reveal OTR Players", &g.session.decloak_players, "Reveals players that are off the radar");
		components::script_patch_checkbox("Reveal Hidden Players", &g.session.unhide_players_from_player_list,"Reveals players that have hidden themselves from the player list");
		components::command_button<"sextall">({}, "Send Sext");
		components::command_button<"fakebanall">({}, "Send Fake Ban Message");
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Off The Radar", &g.session.off_radar_all);
		ImGui::Checkbox("Never Wanted", &g.session.never_wanted_all);
		ImGui::Checkbox("Semi-Godmode", &g.session.semi_godmode_all);
		ImGui::Checkbox("Fix Vehicle", &g.session.vehicle_fix_all);
		components::command_checkbox<"harass">();
		ImGui::Checkbox("Spam Killfeed", &g.session.spam_killfeed);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Explosion Karma", &g.session.explosion_karma);
		ImGui::Checkbox("Damage Karma", &g.session.damage_karma);
		ImGui::Checkbox("Disable Pedestrians", &g.session.disable_peds);
		ImGui::Checkbox("Disable Traffic", &g.session.disable_traffic);
		ImGui::Checkbox("Force Thunder", &g.session.force_thunder);
		ImGui::Checkbox("Lobby Lock", &g.session.lock_session);
		ImGui::SameLine();
		components::help_marker("Blocks all players from joining. May not work on some modders.");
		if (g.session.lock_session)
		{
			ImGui::Checkbox("Allow Friends Into Locked Lobby", &g.session.allow_friends_into_locked_session);
			ImGui::SameLine();
			components::help_marker("Allows Friends to Join Lobby While Locked");
		}
		ImGui::EndGroup();
	}

	void render_force_host_options()
	{
		ImGui::BeginGroup();
		components::small_text("Force Host");

		static constexpr auto token_spoof_types = std::to_array({"Off", "Legit", "Aggressive", "Extra Aggressive", "Custom"});

		ImGui::BeginDisabled(gta_util::get_network()->m_game_session_state != 0);

		ImGui::SetNextItemWidth(300);
		if (ImGui::BeginCombo("Host Token Spoofing",
		        token_spoof_types[g.session.spoof_host_token_type]))
		{
			for (int i = 0; i < token_spoof_types.size(); i++)
			{
				if (ImGui::Selectable(token_spoof_types[i]), i == g.session.spoof_host_token_type)
				{
					g.session.spoof_host_token_type = i;
					g_fiber_pool->queue_job([] {
						g.session.spoof_host_token_dirty = true;
					}); // this part gets a bit racy so we're setting it in a fiber pool
				}

				if (i == g.session.spoof_host_token_type)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if (g.session.spoof_host_token_type != 0)
		{
			ImGui::Checkbox("Hide Token Spoofing When Host", &g.session.hide_token_spoofing_when_host);
		}

		if (g.session.spoof_host_token_type == 4)
		{
			ImGui::SetNextItemWidth(200);
			if (ImGui::InputScalar("##token_input", ImGuiDataType_U64, &g.session.custom_host_token, nullptr, nullptr, "%p", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
			{
				g.session.spoof_host_token_dirty = true;
			}
		}

		ImGui::EndDisabled();

		//if (g.session.force_session_host)
		//{
		//	ImGui::SameLine();
		//	ImGui::Checkbox("Kick Host During Join", &g.session.kick_host_when_forcing_host);
		//}

		ImGui::Checkbox("Force Script Host", &g.session.force_script_host);
		ImGui::SameLine();
		components::help_marker("This might break freemode missions and interiors. Use with caution");

		ImGui::SameLine();

		ImGui::Checkbox("Fast Join", &g.session.fast_join);
		ImGui::SameLine();
		components::help_marker("This WILL break jobs");
		ImGui::Checkbox("Seamless Join", &g.tunables.seamless_join);
		ImGui::SameLine();
		components::help_marker("Allows you move freely while in session transition.");
		components::command_button<"emptysession">();

		ImGui::EndGroup();
	}

	void render_player_options()
	{
		ImGui::SeparatorText("Player Options");

		ImGui::BeginGroup();
		components::command_button<"killall">({}, "Kill Everyone");
		components::command_button<"explodeall">({}, "Explode Everyone");
		components::command_button<"beastall">({});
		ImGui::SameLine();
		components::help_marker("Including you");
		components::command_button<"bringall">({});
		components::command_button<"giveweapsall">({});
		components::command_button<"remweapsall">({});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::command_button<"ceokickall">({});
		components::command_button<"vehkickall">({});
		components::command_button<"deletevehall">({});
		components::command_button<"ragdollall">({}, "Ragdoll Players");
		components::command_button<"intkickall">({}, "Kick Everyone From Interiors");
		components::command_button<"missionall">({});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::command_button<"errorall">({});
		components::command_button<"ceoraidall">({});
		components::button("Trigger MC Raid", [] {
			g_player_service->iterate([](auto& plyr) {
				toxic::start_activity(plyr.second, eActivityType::BikerDefend);
			});
		});
		components::button("Trigger Bunker Raid", [] {
			g_player_service->iterate([](auto& plyr) {
				toxic::start_activity(plyr.second, eActivityType::GunrunningDefend);
			});
		});
		ImGui::EndGroup();

		components::small_text("Bounty");
		static int value = 10000;
		ImGui::SetNextItemWidth(300);
		ImGui::SliderInt("##bountyvalue", &value, 0, 10000);
		components::command_checkbox<"anonbounty">();
		components::button("Bounty All", [] {
			g_player_service->iterate([](auto& plyr) {
				troll::set_bounty_on_player(plyr.second, value, g.session.anonymous_bounty);
			});
		});
	}

	void render_teleport_options()
	{
		ImGui::SeparatorText("Teleports");

		ImGui::SetNextItemWidth(300);
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

		ImGui::SetNextItemWidth(300);
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

		ImGui::BeginGroup();
		components::button("TP All To Darts", [] {
			g_player_service->iterate([](auto& plyr) {
				toxic::start_activity(plyr.second, eActivityType::Darts);
			});
		});
		components::button("TP All To Flight School", [] {
			g_player_service->iterate([](auto& plyr) {
				toxic::start_activity(plyr.second, eActivityType::PilotSchool);
			});
		});
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
		components::command_button<"interiortpall">({81}, "TP All To MOC");
		components::command_button<"interiortpall">({123}, "TP All To Casino");
		components::command_button<"interiortpall">({124}, "TP All To Penthouse");
		components::command_button<"interiortpall">({128}, "TP All To Arcade");
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::command_button<"interiortpall">({146}, "TP All To Music Locker");
		components::command_button<"interiortpall">({148}, "TP All To Record A Studios");
		components::command_button<"interiortpall">({149}, "TP All To Custom Auto Shop");
		components::command_button<"interiortpall">({155}, "TP All To Agency");
		components::command_button<"interiortpall">({160}, "TP All To Freakshop");
		components::command_button<"interiortpall">({161}, "TP All To Multi Floor Garage");
		components::command_button<"tutorialall">();
		components::command_button<"golfall">();
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::command_button<"flightschoolall">();
		components::command_button<"dartsall">();
		components::command_button<"badlandsall">();
		components::command_button<"spacemonkeyall">();
		components::command_button<"wizardall">();
		components::command_button<"qub3dall">();
		components::command_button<"camhedzall">();
		ImGui::EndGroup();
	}

	void render_session_control()
	{
		ImGui::SeparatorText("Session Control");

		ImGui::BeginGroup();
		ImGui::Checkbox("Block Ammunation", &g.session.script_block_opts.ammunation);
		ImGui::Checkbox("Block ATMs", &g.session.script_block_opts.atms);
		ImGui::Checkbox("Block Arcade Games", &g.session.script_block_opts.arcade_games);
		ImGui::Checkbox("Block Casino Games", &g.session.script_block_opts.casino_games);
		ImGui::Checkbox("Block Drones", &g.session.script_block_opts.drones);
		ImGui::Checkbox("Block Gang Attacks", &g.session.script_block_opts.gang_attacks);
		ImGui::Checkbox("Block Deathmatches", &g.session.script_block_opts.impromptu_dm);
		ImGui::Checkbox("Block Impromptu Races", &g.session.script_block_opts.impromptu_race);
		ImGui::Checkbox("Block Interiors", &g.session.script_block_opts.interiors);
		ImGui::SameLine();
		components::help_marker("This does not kick people that are already in interiors");
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Block LSC", &g.session.script_block_opts.lsc);
		ImGui::Checkbox("Block Movies", &g.session.script_block_opts.movies);
		ImGui::Checkbox("Block Prostitutes", &g.session.script_block_opts.prostitutes);
		ImGui::Checkbox("Block Sitting", &g.session.script_block_opts.sitting);
		ImGui::Checkbox("Block Sleeping", &g.session.script_block_opts.sleeping);
		ImGui::Checkbox("Block Stores", &g.session.script_block_opts.stores);
		ImGui::Checkbox("Block Street Dealer", &g.session.script_block_opts.street_dealer);
		ImGui::Checkbox("Block Strip Club", &g.session.script_block_opts.strip_club);
		ImGui::SameLine();
		components::help_marker("This does not always work");
		ImGui::Checkbox("Block Strip Club", &g.session.script_block_opts.vending_machines);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::sub_title("Requires Script Host");
		ImGui::Checkbox("Disable CEO Money", &g.session.block_ceo_money);
		ImGui::SameLine();
		components::help_marker("Blocks CEO money drops across the entire session. This can also break other stuff, use with caution");
		ImGui::Checkbox("Block Jobs", &g.session.block_jobs);
		ImGui::SameLine();
		components::help_marker("Prevents remote players from starting jobs while in your session");
		ImGui::Checkbox("Randomize CEO Colors", &g.session.randomize_ceo_colors);
		components::script_patch_checkbox("Block Muggers", &g.session.block_muggers, "For the entire session");
		components::script_patch_checkbox("Block CEO Raids", &g.session.block_ceo_raids, "For the entire session");
		components::command_checkbox<"blockceos">();
		ImGui::EndGroup();
	}

	void render_session_globals()
	{
		// TODO: what are we doing with this?
		ImGui::BeginGroup();
		components::small_text("Warp Time (requires session host)");

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
		ImGui::EndGroup();

		ImGui::SameLine();

		static int global_wanted_level = 0;

		ImGui::BeginGroup();
		components::small_text("Wanted Level");
		ImGui::SetNextItemWidth(150);
		if (ImGui::SliderInt("##wantedlevel", &global_wanted_level, 0, 5))
		{
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].RemoteWantedLevelAmount = global_wanted_level;
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Force", &g.session.wanted_level_all))
		{
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].RemoteWantedLevelPlayer = __rdtsc() + 32;
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].RemoteWantedLevelAmount = global_wanted_level;
		}
		ImGui::EndGroup();
	}

	void view::network()
	{
		render_join_game();
		render_general_options();
		render_session_globals();
		render_force_host_options();
		render_player_options();
		render_teleport_options();
		render_session_control();
	}
}