#include "backend/reactions/reaction.hpp"
#include "core/data/block_join_reasons.hpp"
#include "views/view.hpp"

namespace big
{
	void draw_pair_option(const std::string_view name, decltype(g.notifications.gta_thread_kill)& option)
	{
		ImGui::TextUnformatted(name.data());

		ImGui::PushID(name.data());
		ImGui::Checkbox("Log", &option.log);
		ImGui::Checkbox("Notify", &option.notify);
		ImGui::PopID();
	}

	void draw_common_reaction(reaction& reaction)
	{
		ImGui::Checkbox("Announce In Chat", &reaction.announce_in_chat);
		ImGui::Checkbox("Notify", &reaction.notify);
		ImGui::Checkbox("Log", &reaction.log);
		ImGui::Checkbox("Add Player To Database", &reaction.add_to_player_db);
		ImGui::Checkbox("Block Joins", &reaction.block_joins);
		ImGui::BeginDisabled(!reaction.block_joins);
		if (ImGui::BeginCombo("Block Join Alert", block_join_reasons[reaction.block_join_reason]))
		{
			for (const auto& [key, value] : block_join_reasons)
			{
				bool is_selected = (reaction.block_join_reason == key);

				if (ImGui::Selectable(value, is_selected))
					reaction.block_join_reason = key;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::EndDisabled();
		ImGui::Checkbox("Kick Attacker", &reaction.kick);
		ImGui::Checkbox("Time Out (Block sync)", &reaction.timeout);
	}

	void draw_reaction(reaction& reaction)
	{
		ImGui::PushID(&reaction);
		if (ImGui::TreeNode(reaction.m_event_name))
		{
			draw_common_reaction(reaction);
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void draw_interloper_reaction(interloper_reaction& reaction)
	{
		ImGui::PushID(&reaction);
		if (ImGui::TreeNode(reaction.m_event_name))
		{
			draw_common_reaction(reaction);

			if (reaction.m_blockable || reaction.m_karmaable)
				ImGui::Separator();

			if (reaction.m_blockable)
				ImGui::Checkbox("Block", &reaction.block);

			if (reaction.m_karmaable)
				ImGui::Checkbox("Karma", &reaction.karma);

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void view::reaction_settings()
	{
		components::title("Reactions");
		draw_reaction(g.reactions.bounty);
		draw_reaction(g.reactions.ceo_kick);
		draw_reaction(g.reactions.ceo_money);
		draw_reaction(g.reactions.chat_spam);
		draw_reaction(g.reactions.clear_ped_tasks);
		draw_reaction(g.reactions.clear_wanted_level);
		draw_reaction(g.reactions.crash);
		draw_reaction(g.reactions.destroy_personal_vehicle);
		draw_reaction(g.reactions.end_session_kick);
		draw_reaction(g.reactions.fake_deposit);
		draw_reaction(g.reactions.force_mission);
		draw_reaction(g.reactions.force_teleport);
		draw_reaction(g.reactions.game_anti_cheat_modder_detection);
		draw_reaction(g.reactions.give_collectible);
		draw_reaction(g.reactions.gta_banner);
		draw_reaction(g.reactions.kick_from_interior);
		draw_reaction(g.reactions.kick_vote);
		draw_reaction(g.reactions.mc_teleport);
		draw_reaction(g.reactions.modder_detection);
		draw_reaction(g.reactions.network_bail);
		draw_reaction(g.reactions.null_function_kick);
		draw_reaction(g.reactions.personal_vehicle_destroyed);
		draw_reaction(g.reactions.remote_off_radar);
		draw_reaction(g.reactions.remote_ragdoll);
		draw_reaction(g.reactions.remote_wanted_level);
		draw_interloper_reaction(g.reactions.remote_wanted_level_others);
		draw_reaction(g.reactions.report);
		draw_reaction(g.reactions.report_cash_spawn);
		draw_reaction(g.reactions.request_control_event);
		draw_reaction(g.reactions.rotate_cam);
		draw_reaction(g.reactions.send_to_cutscene);
		draw_reaction(g.reactions.send_to_interior);
		draw_reaction(g.reactions.send_to_location);
		draw_reaction(g.reactions.sound_spam);
		draw_reaction(g.reactions.spectate);
		draw_reaction(g.reactions.spectate_notification);
		draw_interloper_reaction(g.reactions.spectate_others);
		draw_reaction(g.reactions.start_activity);
		draw_reaction(g.reactions.start_script);
		draw_reaction(g.reactions.teleport_to_warehouse);
		draw_reaction(g.reactions.transaction_error);
		draw_reaction(g.reactions.trigger_business_raid);
		draw_reaction(g.reactions.tse_freeze);
		draw_reaction(g.reactions.tse_sender_mismatch);
		draw_reaction(g.reactions.turn_into_beast);
		draw_reaction(g.reactions.vehicle_kick);

		ImGui::Separator();

		components::title("Notifications");
		ImGui::SeparatorText("GTA Threads");

		draw_pair_option("Terminate", g.notifications.gta_thread_kill);
		draw_pair_option("Start", g.notifications.gta_thread_start);

		ImGui::SeparatorText("Network Player Manager");

		ImGui::TextUnformatted("Player Join");

		ImGui::Checkbox("Above Map", &g.notifications.player_join.above_map);
		ImGui::Checkbox("Log", &g.notifications.player_join.log);
		ImGui::Checkbox("Notify", &g.notifications.player_join.notify);

		draw_pair_option("Player Leave", g.notifications.player_leave);

		draw_pair_option("Init", g.notifications.network_player_mgr_init);
		draw_pair_option("Shutdown", g.notifications.network_player_mgr_shutdown);

		ImGui::SeparatorText("Misc");

		draw_pair_option("Transaction Error / Rate Limit", g.notifications.transaction_rate_limit);
		draw_pair_option("Mismatch sync type", g.notifications.mismatch_sync_type);
		draw_pair_option("Out of allowed range sync type", g.notifications.out_of_allowed_range_sync_type);
		draw_pair_option("Invalid sync", g.notifications.invalid_sync);
	}

}
