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

		inline void IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join)
				src->set_return_value<BOOL>(true);
			else
				src->set_return_value<BOOL>(STREAMING::IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED());
		}

		inline void SET_FOCUS_ENTITY(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join)
				STREAMING::SET_FOCUS_ENTITY(self::ped);
			else
				STREAMING::SET_FOCUS_ENTITY(src->get_arg<Entity>(0));
		}

		inline void HIDE_HUD_AND_RADAR_THIS_FRAME(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join)
				HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		}

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

		inline void TOGGLE_PAUSED_RENDERPHASES(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join)
				GRAPHICS::RESET_PAUSED_RENDERPHASES();
			else
				GRAPHICS::TOGGLE_PAUSED_RENDERPHASES(src->get_arg<int>(0));
		}

		inline void SET_ENTITY_VISIBLE(rage::scrNativeCallContext* src)
		{
			if (g.tunables.seamless_join && src->get_arg<Entity>(0) == self::ped)
				ENTITY::SET_ENTITY_VISIBLE(self::ped, true, false);
			else
				ENTITY::SET_ENTITY_VISIBLE(src->get_arg<Entity>(0), src->get_arg<BOOL>(1), src->get_arg<BOOL>(2));
		}

		inline void SET_ENTITY_COORDS(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join || *scr_globals::transition_state.as<eTransitionState*>() == eTransitionState::TRANSITION_STATE_CONFIRM_FM_SESSION_JOINING || src->get_arg<Entity>(0) != self::ped)
				ENTITY::SET_ENTITY_COORDS(src->get_arg<Entity>(0), src->get_arg<float>(1), src->get_arg<float>(2), src->get_arg<float>(3), src->get_arg<BOOL>(4), src->get_arg<BOOL>(5), src->get_arg<BOOL>(6), src->get_arg<BOOL>(7));
		}

		inline void SET_ENTITY_COLLISION(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join || src->get_arg<Entity>(0) != self::ped)
				ENTITY::SET_ENTITY_COLLISION(src->get_arg<Entity>(0), src->get_arg<BOOL>(1), src->get_arg<BOOL>(2));
		}

		inline void SET_PLAYER_CONTROL(rage::scrNativeCallContext* src)
		{
			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << std::format("PLAYER::SET_PLAYER_CONTROL({}, {}, {});", src->get_arg<Player>(0), src->get_arg<BOOL>(1), src->get_arg<int>(2));

			if (!g.tunables.seamless_join)
				PLAYER::SET_PLAYER_CONTROL(src->get_arg<Player>(0), src->get_arg<BOOL>(1), src->get_arg<int>(2));
		}

		inline void FREEZE_ENTITY_POSITION(rage::scrNativeCallContext* src)
		{
			if (!g.tunables.seamless_join || src->get_arg<Entity>(0) != self::ped)
				ENTITY::FREEZE_ENTITY_POSITION(src->get_arg<Entity>(0), src->get_arg<BOOL>(1));
		}

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

		inline void GET_EVER_HAD_BAD_PACK_ORDER(rage::scrNativeCallContext* src)
		{
			// LOG(VERBOSE) << "DLC::GET_EVER_HAD_BAD_PACK_ORDER(); // Returns: " << DLC::GET_EVER_HAD_BAD_PACK_ORDER();
			src->set_return_value<BOOL>(false);
		}

		inline void ON_ENTER_SP(rage::scrNativeCallContext* src)
		{
			if (g.debug.logs.stupid_script_native_logs)
				LOG(VERBOSE) << "DLC::ON_ENTER_SP();";
		}
	}
}