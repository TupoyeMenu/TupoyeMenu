#include "core/data/language_codes.hpp"
#include "core/data/region_codes.hpp"
#include "fiber_pool.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "services/matchmaking/matchmaking_service.hpp"
#include "services/player_database/player_database_service.hpp"
#include "util/session.hpp"
#include "views/view.hpp"

#include <network/Network.hpp>

namespace big
{
	static int selected_session_idx = -1;

	void view::session_browser()
	{
		static char name_buf[32];
		static char search[64];
		static char session_info[0x100]{};
		ImGui::Text("Total sessions found: %i", g_matchmaking_service->get_num_found_sessions());

		if (ImGui::BeginListBox("###sessions", ImVec2(300, -ImGui::GetFrameHeight())))
		{
			if (g_matchmaking_service->get_num_found_sessions())
			{
				for (int i = 0; i < g_matchmaking_service->get_num_found_sessions(); i++)
				{
					auto& session = g_matchmaking_service->get_found_sessions()[i];

					if (!session.is_valid)
						continue;

					std::string session_str;
					if (session.attributes.multiplex_count > 1)
						session_str = std::format("{:X} (x{})", session.info.m_session_token, session.attributes.multiplex_count);
					else
						session_str = std::format("{:X}", session.info.m_session_token);

					auto host_rid = session.info.m_net_player_data.m_gamer_handle.m_rockstar_id;
					auto player = g_player_database_service->get_player_by_rockstar_id(host_rid);

					if (g.session_browser.exclude_modder_sessions && player && player->block_join)
						continue;

					if (components::selectable(session_str, i == selected_session_idx))
					{
						selected_session_idx = i;
						g_pointers->m_gta.m_encode_session_info(&session.info, session_info, 0xA9, nullptr);
					}

					if (ImGui::IsItemHovered())
					{
						auto tool_tip = std::format("Number of Players: {}\nRegion: {}\nLanguage: {}\nHost Rockstar ID: {}\nDiscriminator: {:X}", session.attributes.player_count,
							regions[session.attributes.region].name,
						    languages[session.attributes.language].name,
						    session.info.m_net_player_data.m_gamer_handle.m_rockstar_id, // TODO: this is not accurate
						    session.attributes.discriminator);
						ImGui::SetTooltip("%s", tool_tip.c_str());
					}
				}
			}
			else
			{
				ImGui::TextUnformatted("No sessions");
			}

			ImGui::EndListBox();
		}

		if (selected_session_idx != -1)
		{
			ImGui::SameLine();
			if (ImGui::BeginChild("###selected_session", ImVec2(300, -ImGui::GetFrameHeight()), false, ImGuiWindowFlags_NoBackground))
			{
				auto& session = g_matchmaking_service->get_found_sessions()[selected_session_idx];

				ImGui::Text("Num Players: %d", session.attributes.player_count);
				ImGui::Text("Discriminator: 0x%X", session.attributes.discriminator);
				ImGui::Text("Region: %s", regions[session.attributes.region].name);
				ImGui::Text("Language: %s", languages[session.attributes.language].name);

				auto& data = session.info.m_net_player_data;
				ImGui::Text("Host Rockstar ID: %llu", data.m_gamer_handle.m_rockstar_id);

				components::button("Copy Session Info", [] {
					ImGui::SetClipboardText(session_info);
				});
				ImGui::SameLine();
				components::button("Join", [session] {
					if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_J) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
					{
						g_notification_service.push_error("Join Session", "Player switch in progress, wait a bit.");
						return;
					}

					bool is_session_free_aim = session.attributes.discriminator & (1 << 17);
					bool is_local_free_aim   = PAD::GET_LOCAL_PLAYER_GAMEPAD_AIM_STATE() > 1;

					if (is_session_free_aim != is_local_free_aim)
						PLAYER::SET_PLAYER_TARGETING_MODE(is_session_free_aim ? 3 : 1);

					session::join_session(session.info);
				});
			}
			ImGui::EndChild();
		}

		if (ImGui::TreeNode("Filters"))
		{
			ImGui::Checkbox("Region", &g.session_browser.region_filter_enabled);
			ImGui::SameLine();
			components::help_marker("It is highly recommended to keep this filter enabled");

			if (g.session_browser.region_filter_enabled)
			{
				ImGui::SameLine();

				if (ImGui::BeginCombo("###region_select", regions[g.session_browser.region_filter].name))
				{
					for (const auto& region : regions)
					{
						if (ImGui::Selectable(region.name, g.session_browser.region_filter == region.id))
						{
							g.session_browser.region_filter = region.id;
						}
					}
					ImGui::EndCombo();
				}
			}

			ImGui::Checkbox("Language", &g.session_browser.language_filter_enabled);
			ImGui::SameLine();
			components::help_marker("Setting a correct region filter for the language will help tremendously");

			if (g.session_browser.language_filter_enabled)
			{
				ImGui::SameLine();

				if (ImGui::BeginCombo("###language_select", languages[g.session_browser.language_filter].name))
				{
					for (const auto& language : languages)
					{
						if (ImGui::Selectable(language.name, g.session_browser.language_filter == language.id))
						{
							g.session_browser.language_filter = language.id;
						};
					}
					ImGui::EndCombo();
				}
			}

			ImGui::Checkbox("Players", &g.session_browser.player_count_filter_enabled);

			if (g.session_browser.player_count_filter_enabled)
			{
				ImGui::InputInt("Minimum", &g.session_browser.player_count_filter_minimum);
				ImGui::InputInt("Maximum", &g.session_browser.player_count_filter_maximum);
			}

			ImGui::Checkbox("Pool Type", &g.session_browser.pool_filter_enabled);
			if (g.session_browser.pool_filter_enabled)
			{
				ImGui::SameLine();
				ImGui::Combo("###pooltype", &g.session_browser.pool_filter, "Normal\0Bad Sport\0");
			}

			ImGui::Checkbox("FILTER_MULTIPLEXED_SESSIONS", &g.session_browser.filter_multiplexed_sessions);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("FILTER_MULTIPLEXED_SESSIONS_DESC");
      
			ImGui::Checkbox("Exclude Modder Sessions", &g.session_browser.exclude_modder_sessions);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Excludes hosts that you have blocked in the Player Database");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sorting"))
		{
			ImGui::Combo("Sort By", &g.session_browser.sort_method, "Off\0Player Count\0");
			if (g.session_browser.sort_method != 0)
				ImGui::Combo("Direction", &g.session_browser.sort_direction, "Ascending\0Descending\0");
			ImGui::TreePop();
		}

		ImGui::Checkbox("Replace Game Matchmaking", &g.session_browser.replace_game_matchmaking);
		ImGui::SameLine();
		components::help_marker("This will replace the default game matchmaking with a custom one that will use the filters and sorting set here");

		components::button("Refresh", [] {
			selected_session_idx = -1;

			if (!g_matchmaking_service->matchmake())
				g_notification_service.push_error("Matchmaking", "Matchmaking failed");
		});
	}
}
