/**
 * @file troll.hpp
 * @brief Trolling functions on other players.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "gta/enums.hpp"
#include "pointers.hpp"
#include "script_global.hpp"
#include "services/players/player_service.hpp"

namespace big::troll
{
	/**
	 * @brief Sets a bounty on player.
	 * 
	 * @param target Player to set bounty on.
	 * @param value How much GTA$ do you want a bounty to be.
	 * @note Max value is 10000, max legal value is 9000
	 * 
	 * @param anonymous Don't show your name when setting a bounty.
	 */
	inline void set_bounty_on_player(player_ptr target, int value, bool anonymous)
	{
		const size_t arg_count  = 22;
		int64_t args[arg_count] = {(int64_t)eRemoteEvent::Bounty,
		    self::id,
		    target->id(),
		    1,
		    value,
		    0,
		    anonymous ? 1 : 0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    0,
		    *script_global(1923597).at(9).as<int64_t*>(),
		    *script_global(1923597).at(10).as<int64_t*>()};

		g_pointers->m_gta.m_trigger_script_event(1, args, arg_count, 1 << target->id());
	}

}
