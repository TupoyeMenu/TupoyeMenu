#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "util/troll.hpp"
#include "util/toxic.hpp"
#include "views/view.hpp"

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

	void render_player_options()
	{
		ImGui::SeparatorText("Player Options");

		ImGui::BeginGroup();
		components::command_button<"killall">({}, "Kill Everyone");
		components::command_button<"explodeall">({}, "Explode Everyone");
		components::command_button<"beastall">({});
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Including you");
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
		ImGui::Checkbox("Block Vending Machines", &g.session.script_block_opts.vending_machines);
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
				if (ImGui::Selectable(token_spoof_types[i], i == g.session.spoof_host_token_type))
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
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("This cannot be reversed. Use with caution");
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

	void view::network_controls()
	{
		render_player_options();
		render_session_control();
		render_force_host_options();
		render_session_globals();
	}
}
