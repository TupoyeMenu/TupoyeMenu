#pragma once
#include "core/data/infractions.hpp"
#include "gta/joaat.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "rage/rlSessionByGamerTaskResult.hpp"
#include "script.hpp"
#include "script_global.hpp"
#include "services/player_database/player_database_service.hpp"
#include "services/players/player_service.hpp"
#include "util/misc.hpp"
#include "script_function.hpp"

#include <network/Network.hpp>
#include <network/snConnectToPeerTask.hpp>
#include <rage/rlQueryPresenceAttributesContext.hpp>
#include <rage/rlScHandle.hpp>
#include <script/globals/GPBD_FM_3.hpp>

namespace big::session
{
	/**
	 * @brief Join session by type.
	 * 
	 * @param session Session type. 
	 */
	inline void join_type(eSessionType session)
	{
		*scr_globals::sctv_spectator.as<int*>() = (session == eSessionType::SC_TV ? 1 : 0); // If SCTV then enable spectator mode

		if (session == eSessionType::LEAVE_ONLINE)
			*scr_globals::session.at(2).as<int*>() = -1;
		else
		{
			*scr_globals::session.at(2).as<int*>() = 0;
			*scr_globals::session2.as<int*>()      = (int)session;
		}

		*scr_globals::session.as<int*>() = 1;

		if (*g_pointers->m_gta.m_is_session_started && session != eSessionType::LEAVE_ONLINE)
		{
			*scr_globals::transition_state.as<eTransitionState*>() = eTransitionState::TRANSITION_STATE_LOOK_TO_JOIN_ANOTHER_SESSION_FM;
		}
		else if (session == eSessionType::LEAVE_ONLINE)
		{
			*scr_globals::transition_state.as<eTransitionState*>() = eTransitionState::TRANSITION_STATE_RETURN_TO_SINGLEPLAYER;
		}

		scr_functions::reset_session_data.call<void>(true, true);
		*scr_globals::session3.as<int*>() = 0;
		*scr_globals::session4.as<int*>() = 1;
		*scr_globals::session5.as<int*>() = 32;

		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_J) == 0)
		{
			*scr_globals::session6.as<int*>() = 1;
			script::get_current()->yield(500ms);
			*scr_globals::session.as<int*>() = 0;
		}
	}

	inline void set_fm_event_index(int index)
	{
		int idx = index / 32;
		int bit = index % 32;
		misc::set_bit(scr_globals::gsbd_fm_events.at(11).at(379).at(idx, 1).as<int*>(), bit);
		misc::set_bit(scr_globals::gsbd_fm_events.at(11).at(370).at(idx, 1).as<int*>(), bit);
		misc::set_bit((int*)&scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[self::id].BossGoon.ActiveFreemodeEvents[idx], bit);
	}

	inline void clear_fm_event_index(int index)
	{
		int idx = index / 32;
		int bit = index % 32;
		misc::clear_bit(scr_globals::gsbd_fm_events.at(11).at(379).at(idx, 1).as<int*>(), bit);
		misc::clear_bit(scr_globals::gsbd_fm_events.at(11).at(370).at(idx, 1).as<int*>(), bit);
		misc::clear_bit((int*)&scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[self::id].BossGoon.ActiveFreemodeEvents[idx], bit);
	}

	/**
	 * @brief Joins session by is't rage::rlSessionInfo.
	 * 
	 * @param info Session info to join.
	 */
	inline void join_session(const rage::rlSessionInfo& info)
	{
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_J) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
		{
			g_notification_service.push_error("Rid Joiner", "Player switch in progress, wait a bit.");
			return;
		}

		g.session.join_queued = true;
		g.session.info        = info;
		session::join_type(eSessionType::NEW_PUBLIC);
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_J) == 0)
		{
			g.session.join_queued = false;
			g_notification_service.push_error("Rid Joiner", "Unable to launch maintransition");
		}
		return;
	}

	/**
	 * @brief Joines player by Rockstar ID.
	 * @param rid RID to join.
	*/
	inline void join_by_rockstar_id(uint64_t rid)
	{
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_J) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
		{
			g_notification_service.push_error("Rid Joiner", "Player switch in progress, wait a bit.");
			return;
		}

		rage::rlGamerHandle player_handle(rid);
		rage::rlSessionByGamerTaskResult result;
		bool success = false;
		rage::rlTaskStatus state{};

		if (g_pointers->m_gta.m_start_get_session_by_gamer_handle(0, &player_handle, 1, &result, 1, &success, &state))
		{
			while (state.status == 1)
				script::get_current()->yield();

			if (state.status == 3 && success)
			{
				join_session(result.m_session_info);
				return;
			}
		}

		g_notification_service.push_error("Rid Joiner", "Target player is offline?");
	}

	inline void invite_by_rockstar_id(uint64_t rid)
	{
		rage::rlGamerHandle player_handle(rid);

		bool success = g_pointers->m_gta.m_invite_player_by_gamer_handle(*g_pointers->m_gta.m_network, &player_handle, 1, nullptr, nullptr, nullptr);

		if (!success)
			return g_notification_service.push_error("Network", "RID_JOINER_INVITE_OFFLINE");

		g_notification_service.push_success("Network", "Target player has been invited to your session!");
	}

	inline void show_profile_by_rockstar_id(uint64_t rid)
	{
		rage::rlGamerHandle player_handle(rid);

		g_pointers->m_gta.m_show_profile_by_gamer_handle(&player_handle);
	}

	inline void add_friend_by_rockstar_id(uint64_t rid)
	{
		rage::rlGamerHandle player_handle(rid);

		g_pointers->m_gta.m_add_friend_by_gamer_handle(&player_handle, 0);
	}


	/**
	 * @brief Adds player to the database and assigns infraction.
	 * 
	 * @param player Player to add infraction to.
	 * @param infraction Infraction to add.
	 * @param custom_reason Text of the infration.
	 */
	inline void add_infraction(player_ptr player, Infraction infraction, const std::string& custom_reason = "")
	{
		if (g.debug.fuzzer.enabled)
			return;
		if ((player->is_friend() && g.session.trust_friends) || player->is_trusted || g.session.trust_session)
			return;

		auto plyr = g_player_database_service->get_or_create_player(player);
		if (!plyr->infractions.contains((int)infraction))
		{
			plyr->is_modder   = true;
			player->is_modder = true;

			LOG(INFO) << std::format("Anti-Cheat: {} - {}", player->get_name(), plyr->get_infraction_description((int)infraction));

			plyr->infractions.insert((int)infraction);
			if (infraction == Infraction::CUSTOM_REASON)
			{
				plyr->custom_infraction_reason += plyr->custom_infraction_reason.size() ? (std::string(", ") + custom_reason) : custom_reason;
			}

			g_player_database_service->save();

			g.reactions.modder_detection.process(player);
		}
	}

	inline void give_collectible(Player target, eCollectibleType col, int index = 0, bool uncomplete = false)
	{
		const size_t arg_count  = 8;
		int64_t args[arg_count] = {
		    (int64_t)eRemoteEvent::GiveCollectible,
		    (int64_t)self::id,
		    1 << target,
		    (int64_t)col,   // iParam0
		    (int64_t)index, // iParam1
		    !uncomplete,    // bParam2
		    true,
		    0 // bParam3
		};

		g_pointers->m_gta.m_trigger_script_event(1, args, arg_count, 1 << target, (int)eRemoteEvent::GiveCollectible);
	}
}
