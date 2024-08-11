#include "backend/bool_command.hpp"
#include "core/data/apartment_names.hpp"
#include "core/data/region_codes.hpp"
#include "core/data/warehouse_names.hpp"
#include "gta_util.hpp"
#include "gui/components/components.hpp"
#include "hooking/hooking.hpp"
#include "util/session.hpp"
#include "util/toxic.hpp"
#include "views/view.hpp"

#include <network/Network.hpp>
#include <script/globals/GPBD_FM_3.hpp>


namespace big
{
	struct SessionType
	{
		eSessionType id;
		const char* name;
	};

	static uint64_t rid = 0;

	void render_join_game()
	{
		ImGui::SeparatorText("Join Game");

		ImGui::BeginGroup();
		
		ImGui::SetNextItemWidth(200);
		bool rid_submitted = ImGui::InputScalar("##inputrid", ImGuiDataType_U64, &rid, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::SameLine();
		if(components::button("Join by RID") || rid_submitted)
		{
			const auto rid_lambda = rid;
			g_fiber_pool->queue_job([rid_lambda]() {
				session::join_by_rockstar_id(rid_lambda);
			});
		}

		static char base64[500]{};
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
		components::help_marker("Select your region for the session");

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

	void view::network()
	{
		render_join_game();
		render_general_options();
		render_teleport_options();
	}
}