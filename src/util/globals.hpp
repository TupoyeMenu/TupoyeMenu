/**
 * @file globals.hpp
 * @brief Features that use script globals.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "core/scr_globals.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script_global.hpp"

#include <script/globals/GPBD_FM_3.hpp>
#include <script/globals/GlobalPlayerBD.hpp>

namespace big::globals
{
	/**
	 * @brief Clears wanted level of target using script events.
	 * 
	 * @param target Player to clear wanted level of.
	 */
	inline void clear_wanted_player(Player target)
	{
		constexpr size_t arg_count = 4;
		int64_t args[arg_count]    = {static_cast<int64_t>(eRemoteEvent::ClearWantedLevel),
		       self::id, 1 << target,
		       (int64_t)scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[target].ScriptEventReplayProtectionCounter};

		g_pointers->m_gta.m_trigger_script_event(1, args, arg_count, 1 << target, (int)eRemoteEvent::ClearWantedLevel);
	}

	/**
	 * @brief Gives off the radar effect to the target.
	 * 
	 * @param target Player to give off the radar effect to.
	 */
	inline void give_remote_otr(Player target)
	{
		constexpr size_t arg_count = 8;
		int64_t args[arg_count]    = {static_cast<int64_t>(eRemoteEvent::RemoteOffradar),
		       (int64_t)self::id,
		       1 << target,
		       (int64_t)(NETWORK::GET_NETWORK_TIME() + 1),
		       0,
		       true,
		       false,
		       (int64_t)scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[target].ScriptEventReplayProtectionCounter};

		g_pointers->m_gta.m_trigger_script_event(1, args, arg_count, 1 << target, (int)eRemoteEvent::RemoteOffradar);
	}

	/**
	 * @brief Gets the interior from player.
	 * 
	 * @note This can probably be spoofed by remote players, although it unlikely.
	 * 
	 * @param player Player to get interior of.
	 * @return Interior player is currently in.
	 */
	inline Interior get_interior_from_player(Player player)
	{
		return scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[player].CurrentInteriorIndex;
	}
}
