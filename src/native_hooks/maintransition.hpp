/**
 * @file maintransition.hpp
 * @brief Native hooks for the maintransition script.
 * Mosly contains seamless switch code.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "core/enums.hpp"
#include "core/globals.hpp"
#include "gta_util.hpp"
#include "native_hooks.hpp"
#include "natives.hpp"
#include "pointers.hpp"

namespace big
{
	namespace maintransition
	{
		inline void NETWORK_SESSION_HOST(rage::scrNativeCallContext* src)
		{
			if (g.session.join_queued)
			{
				g_pointers->m_gta.m_join_session_by_info(gta_util::get_network(), &g.session.info, 1, 1 | 2, nullptr, 0);
				g.session.join_queued = false;
				src->set_return_value<BOOL>(TRUE);
			}
			else
			{
				if (g.debug.logs.stupid_script_native_logs)
					LOG(VERBOSE) << std::format("NETWORK::NETWORK_SESSION_HOST({}, {}, {});", src->get_arg<int>(0), src->get_arg<int>(1), src->get_arg<BOOL>(2));
				src->set_return_value<BOOL>(NETWORK::NETWORK_SESSION_HOST(src->get_arg<int>(0), src->get_arg<int>(1), src->get_arg<BOOL>(2)));
			}
		};

		//
		// PLAYER_SWITCH
		//

		/**
		 * @brief This hook lets you stop player-switch in "Pre-HUD Checks"
		 */
		inline void IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join)
				src->set_return_value<BOOL>(true);
			else
				src->set_return_value<BOOL>(STREAMING::IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED());
		}

		/**
		 * @brief Prevets map from unloading.
		 */
		inline void SET_FOCUS_ENTITY(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join)
				STREAMING::SET_FOCUS_ENTITY(self::ped);
			else
				STREAMING::SET_FOCUS_ENTITY(src->get_arg<Entity>(0));
		}

		/**
		 * @brief Draw hud and radar in transition.
		 * @bug Doesn't work.
		 */
		inline void HIDE_HUD_AND_RADAR_THIS_FRAME(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join)
				HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		}

		/**
		 * @brief Let's you controll your ped when going sp to mp.
		 */
		inline void ACTIVATE_FRONTEND_MENU(rage::scrNativeCallContext* src)
		{
			Hash menuhash = src->get_arg<Hash>(0);
			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << std::format("HUD::ACTIVATE_FRONTEND_MENU({}, {}, {});", menuhash, src->get_arg<BOOL>(1), src->get_arg<int>(2));

			if (g.tunables.seamless_join && menuhash != RAGE_JOAAT("FE_MENU_VERSION_EMPTY_NO_BACKGROUND"))
				HUD::ACTIVATE_FRONTEND_MENU(menuhash, src->get_arg<BOOL>(1), src->get_arg<int>(2));

			if (!g.tunables.seamless_join)
				HUD::ACTIVATE_FRONTEND_MENU(menuhash, src->get_arg<BOOL>(1), src->get_arg<int>(2));
		}

		/**
		 * @brief Let's you controll your ped when going sp to mp.
		 */
		inline void RESTART_FRONTEND_MENU(rage::scrNativeCallContext* src)
		{
			Hash menuhash = src->get_arg<Hash>(0);

			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << std::format("HUD::RESTART_FRONTEND_MENU({}, {});", menuhash, src->get_arg<int>(1));

			if (g.tunables.seamless_join)
			{
				if (menuhash != RAGE_JOAAT("FE_MENU_VERSION_EMPTY_NO_BACKGROUND"))
				{
					HUD::RESTART_FRONTEND_MENU(menuhash, src->get_arg<int>(1));
				}
			}
			else
			{
				HUD::RESTART_FRONTEND_MENU(menuhash, src->get_arg<int>(1));
			}
		}

		/**
		 * @brief Prevents the game from freezing your screen.
		 */
		inline void TOGGLE_PAUSED_RENDERPHASES(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join)
				GRAPHICS::RESET_PAUSED_RENDERPHASES();
			else
				GRAPHICS::TOGGLE_PAUSED_RENDERPHASES(src->get_arg<int>(0));
		}

		/**
		 * @brief Makes you visible.
		 */
		inline void SET_ENTITY_VISIBLE(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join && src->get_arg<Entity>(0) == self::ped)
				ENTITY::SET_ENTITY_VISIBLE(self::ped, true, false);
			else
				ENTITY::SET_ENTITY_VISIBLE(src->get_arg<Entity>(0), src->get_arg<BOOL>(1), src->get_arg<BOOL>(2));
		}

		/**
		 * @brief Prevents the game from teleporting you.
		 */
		inline void SET_ENTITY_COORDS(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join || *scr_globals::transition_state.as<eTransitionState*>() == eTransitionState::TRANSITION_STATE_CONFIRM_FM_SESSION_JOINING || src->get_arg<Entity>(0) != self::ped)
				ENTITY::SET_ENTITY_COORDS(src->get_arg<Entity>(0), src->get_arg<float>(1), src->get_arg<float>(2), src->get_arg<float>(3), src->get_arg<BOOL>(4), src->get_arg<BOOL>(5), src->get_arg<BOOL>(6), src->get_arg<BOOL>(7));
		}

		/**
		 * @brief Prevents you from falling.
		 */
		inline void SET_ENTITY_COLLISION(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join || src->get_arg<Entity>(0) != self::ped)
				ENTITY::SET_ENTITY_COLLISION(src->get_arg<Entity>(0), src->get_arg<BOOL>(1), src->get_arg<BOOL>(2));
		}

		/**
		 * @brief Allows controll in session switch.
		 */
		inline void SET_PLAYER_CONTROL(rage::scrNativeCallContext* src)
		{
			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << std::format("PLAYER::SET_PLAYER_CONTROL({}, {}, {});", src->get_arg<Player>(0), src->get_arg<BOOL>(1), src->get_arg<int>(2));

			if (!g.tunables.seamless_join)
				PLAYER::SET_PLAYER_CONTROL(src->get_arg<Player>(0), src->get_arg<BOOL>(1), src->get_arg<int>(2));
		}

		/**
		 * @brief Allows controll in session switch.
		 */
		inline void FREEZE_ENTITY_POSITION(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join || src->get_arg<Entity>(0) != self::ped)
				ENTITY::FREEZE_ENTITY_POSITION(src->get_arg<Entity>(0), src->get_arg<BOOL>(1));
		}

		/**
		 * @brief Prevents player from teleporting after switch.
		 */
		inline void NETWORK_RESURRECT_LOCAL_PLAYER(rage::scrNativeCallContext* src)
		{
			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << std::format("NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER({}, {}, {}, {}, {}, {}, {}, {}, {});", src->get_arg<float>(0), src->get_arg<float>(1), src->get_arg<float>(2), src->get_arg<float>(3), src->get_arg<BOOL>(4), src->get_arg<BOOL>(5), src->get_arg<BOOL>(6), src->get_arg<int>(7), src->get_arg<int>(8));

			if (!g.tunables.seamless_join)
				NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(src->get_arg<float>(0), src->get_arg<float>(1), src->get_arg<float>(2), src->get_arg<float>(3), src->get_arg<BOOL>(4), src->get_arg<BOOL>(5), src->get_arg<BOOL>(6), src->get_arg<int>(7), src->get_arg<int>(8));
		}

		//
		// PLAYER_SWITCH END
		//

		/**
		 * @brief Prevent weird reloading when using custom dlcs.
		 * @note You should not be using custom dlcs like this in the first place.
		 */
		inline void GET_EVER_HAD_BAD_PACK_ORDER(rage::scrNativeCallContext* src)
		{
			// LOG(VERBOSE) << "DLC::GET_EVER_HAD_BAD_PACK_ORDER(); // Returns: " << DLC::GET_EVER_HAD_BAD_PACK_ORDER();
			src->set_return_value<BOOL>(false);
		}

		/**
		 * @brief Prevent single player map from loading when going back from online.
		 */
		inline void ON_ENTER_SP(rage::scrNativeCallContext* src)
		{
			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << "DLC::ON_ENTER_SP();";

			if(!g.tunables.dont_unload_online_maps)
				DLC::ON_ENTER_SP();
		}

		/**
		 * 
		 * @param src 
		 */
		inline void CLEAR_PED_TASKS_IMMEDIATELY(rage::scrNativeCallContext* src)
		{
			if (src->get_arg<Ped>(0) == self::ped && g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << std::format("TASK::CLEAR_PED_TASKS_IMMEDIATELY({}); // In: {}", src->get_arg<Ped>(0), SCRIPT::GET_THIS_SCRIPT_NAME());

			if (!g.tunables.seamless_join)
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(src->get_arg<Ped>(0));
		}
	}
}