#pragma once
#include "core/enums.hpp"
#include "core/scr_globals.hpp"
#include "gta/net_object_mgr.hpp"
#include "gta/pickup_rewards.hpp"
#include "pointers.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "util/entity.hpp"
#include "util/scripts.hpp"
#include "util/session.hpp"
#include "util/system.hpp"

#include <network/Network.hpp>
#include <network/netTime.hpp>
#include <script/globals/GPBD_FM_3.hpp>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

namespace big::toxic
{
	inline void blame_explode_coord(player_ptr to_blame, Vector3 pos, eExplosionTag explosion_type, float damage, bool is_audible, bool is_invisible, float camera_shake)
	{
		g_pointers->m_blame_explode->apply();
		FIRE::ADD_OWNED_EXPLOSION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(to_blame->id()), pos.x, pos.y, pos.z, (int)explosion_type, damage, is_audible, is_invisible, camera_shake);

		g_pointers->m_blame_explode->restore();
	}

	inline void blame_explode_player(player_ptr to_blame, player_ptr target, eExplosionTag explosion_type, float damage, bool is_audible, bool is_invisible, float camera_shake)
	{
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->id()), true);
		blame_explode_coord(to_blame, coords, explosion_type, damage, is_audible, is_invisible, camera_shake);
	}

	inline void start_activity(player_ptr target, eActivityType type)
	{
		const size_t arg_count = 4;
		int64_t args[arg_count] = {(int64_t)eRemoteEvent::StartActivity, (int64_t)self::id, (int64_t)type, (int64_t) true};

		g_pointers->m_trigger_script_event(1, args, arg_count, 1 << target->id());
	}

	inline bool set_time(player_ptr target, uint32_t millis)
	{
		if (!g_player_service->get_self()->is_host())
		{
			g_notification_service->push_error("MODIFY_TIME"_T.data(), "MODIFY_TIME_HOST_REQUIRED"_T.data());
			return false;
		}

		if (!target->player_time_value.has_value())
		{
			g_notification_service->push_error("MODIFY_TIME"_T.data(), "MODIFY_TIME_NO_PLAYER_TIMESTAMP"_T.data());
			return false;
		}

		target->num_time_syncs_sent++;

		rage::netTimeSyncMsg msg{};
		msg.action    = 1;
		msg.counter   = target->num_time_syncs_sent;
		msg.token     = (*g_pointers->m_network_time)->m_time_token;
		msg.timestamp = target->player_time_value.value()
		    + (uint32_t)(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
		        - target->player_time_value_received_time.value())
		          .count();
		msg.increment = millis;

		auto peer = g_pointers->m_get_connection_peer(gta_util::get_network()->m_game_session_ptr->m_net_connection_mgr,
		    (int)target->get_session_player()->m_player_data.m_peer_id_2);

		for (int j = 0; j < 100; j++)
		{
			g_pointers->m_sync_network_time(gta_util::get_network()->m_game_session_ptr->m_net_connection_mgr,
			    peer,
			    (*g_pointers->m_network_time)->m_connection_identifier,
			    &msg,
			    0x1000000); // repeatedly spamming the event will eventually cause certain bounds checks to disable for some reason
		}

		return true;
	}

	inline void warp_time_forward(player_ptr target, uint32_t millis)
	{
		if (!target->player_time_value.has_value())
		{
			g_notification_service->push_error("WARP_TIME_TITLE"_T.data(), "MODIFY_TIME_NO_PLAYER_TIMESTAMP"_T.data());
			return;
		}

		if (set_time(target,
		        target->time_difference.value() + millis
		            + (uint32_t)(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
		                - target->player_time_value_received_time.value())
		                  .count()))
			target->time_difference.value() += millis;
	}

	inline void set_time_all(uint32_t millis)
	{
		if (!g_player_service->get_self()->is_host())
		{
			g_notification_service->push_error("MODIFY_TIME"_T.data(), "MODIFY_TIME_HOST_REQUIRED"_T.data());
			return;
		}

		std::uint32_t largest_counter = 9999;
		g_player_service->iterate([&largest_counter](const player_entry& plyr) {
			if (plyr.second->num_time_syncs_sent > largest_counter)
				largest_counter = plyr.second->num_time_syncs_sent;
		});

		(*g_pointers->m_network_time)->m_time_offset = millis - timeGetTime();

		rage::netTimeSyncMsg msg{};
		g_player_service->iterate([&largest_counter, &msg, millis](const player_entry& plyr) {
			if (!plyr.second->player_time_value.has_value())
			{
				LOG(WARNING) << "Skipping " << plyr.second->get_name() << " in time warp";
				return;
			}

			largest_counter++;

			msg.action    = 1;
			msg.counter   = largest_counter;
			msg.token     = (*g_pointers->m_network_time)->m_time_token;
			msg.timestamp = plyr.second->player_time_value.value()
			    + (uint32_t)(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
			        - plyr.second->player_time_value_received_time.value())
			          .count();
			msg.increment = millis;

			auto peer = g_pointers->m_get_connection_peer(gta_util::get_network()->m_game_session_ptr->m_net_connection_mgr,
			    (int)plyr.second->get_session_player()->m_player_data.m_peer_id_2);

			for (int j = 0; j < 25; j++)
			{
				g_pointers->m_sync_network_time(gta_util::get_network()->m_game_session_ptr->m_net_connection_mgr,
				    peer,
				    (*g_pointers->m_network_time)->m_connection_identifier,
				    &msg,
				    0x1000000);
			}

			plyr.second->num_time_syncs_sent = largest_counter + 32;
		});
	}

	inline void warp_time_forward_all(uint32_t millis)
	{
		set_time_all((*g_pointers->m_network_time)->m_time + millis);
	}

	inline void ragdoll_player_old(player_ptr target)
	{
		auto pos = target->get_ped()->get_position();
		FIRE::ADD_EXPLOSION(pos->x, pos->y, pos->z, (int)eExplosionTag::DIR_WATER_HYDRANT, 1, false, true, 0, false);
	}

	inline void full_acceleration(player_ptr target)
	{
		if (!target->get_ped())
			return;

		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->id()), true);
		if (entity::take_control_of(veh))
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, VEHICLE::GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(ENTITY::GET_ENTITY_MODEL(veh)) * 3);
	}

	inline void launch_vehicle_up(player_ptr target)
	{
		if (!target->get_ped())
			return;

		Entity ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->id());

		if (!PED::IS_PED_IN_ANY_VEHICLE(ent, true))
			g_notification_service->push_warning("Toxic", "Target player is not in a vehicle.");
		else
		{
			ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

			if (entity::take_control_of(ent))
				ENTITY::APPLY_FORCE_TO_ENTITY(ent, 1, 0.f, 0.f, 3000, 0.f, 0.f, 0.f, 0, 0, 1, 1, 0, 1);
			else
				g_notification_service->push_warning("Toxic", "Failed to take control of player vehicle.");
		}
	}
}
