/**
 * @file freemode.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "native_hooks.hpp"
#include "natives.hpp"
#include "script_function.hpp"

namespace big
{
	namespace freemode
	{
		void GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(rage::scrNativeCallContext* src)
		{
			const auto hash = src->get_arg<rage::joaat_t>(0);

			if (hash == RAGE_JOAAT("director_mode") || hash == RAGE_JOAAT("main"))
			{
				src->set_return_value(0);
				return;
			}

			src->set_return_value(SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(hash));
		}

		inline void IS_PLAYER_PLAYING(rage::scrNativeCallContext* src)
		{
			// block undead OTR
			if (g.session.decloak_players && src->get_arg<Player>(0) != self::id && !NETWORK::NETWORK_IS_ACTIVITY_SESSION())
				src->set_return_value<BOOL>(TRUE);
			else
				src->set_return_value<BOOL>(PLAYER::IS_PLAYER_PLAYING(src->get_arg<Player>(0)));
		};

		inline void SET_ENTITY_VISIBLE(rage::scrNativeCallContext* src)
		{
			auto entity = src->get_arg<Entity>(0);
			auto toggle = src->get_arg<bool>(1);
			auto outfit = src->get_arg<bool>(2);

			if (g.self.invisibility && entity == self::ped && toggle)
				return;
			else
				ENTITY::SET_ENTITY_VISIBLE(entity, toggle, outfit);
		}

		void SET_BIGMAP_ACTIVE(rage::scrNativeCallContext* src)
		{
			if (!g.m_mission_creator_thread)
				HUD::SET_BIGMAP_ACTIVE(src->get_arg<BOOL>(0), src->get_arg<BOOL>(1));
		};

		void SET_BLIP_DISPLAY(rage::scrNativeCallContext* src)
		{
			if ((!g.m_mission_creator_thread) || src->get_arg<Blip>(0) != HUD::GET_MAIN_PLAYER_BLIP_ID())
				HUD::SET_BLIP_DISPLAY(src->get_arg<Blip>(0), src->get_arg<BOOL>(1));
		};

		void NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA(rage::scrNativeCallContext* src)
		{
			if (NETWORK::NETWORK_IS_ACTIVITY_SESSION() || NETWORK::NETWORK_IS_IN_TRANSITION() || NETWORK::NETWORK_IS_TRANSITION_TO_GAME() || NETWORK::NETWORK_IS_HOST_OF_THIS_SCRIPT())
			{
				src->set_return_value<BOOL>(NETWORK::NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA());
			}
			else
			{
				if (g.session.force_script_host && (g.session.fast_join || NETWORK::NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA()))
				{
					auto hash = SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME();
					g_fiber_pool->queue_job([hash] {
						scripts::force_host(hash);
						if (auto script = gta_util::find_script_thread(hash); script && script->m_net_component)
							script->m_net_component->block_host_migration(true);
					});
				}

				if (SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME() == RAGE_JOAAT("freemode") && g.session.fast_join)
				{
					scr_functions::set_freemode_session_active({});
					src->set_return_value<BOOL>(TRUE);
				}
				else
				{
					src->set_return_value<BOOL>(NETWORK::NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA());
				}
			}
		}
	}
}