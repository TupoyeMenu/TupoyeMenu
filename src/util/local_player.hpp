/**
 * @file local_player.hpp
 * @brief Basic local player related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "core/data/levels.hpp"
#include "gta/joaat.hpp"
#include "natives.hpp"
#include "script_global.hpp"

namespace big::local_player
{
	/**
	 * @brief Gets the current GTAO character slot.
	 * 
	 * @param character_slot Outputs currently selected GTA Online character.
	 */
	inline void get_active_character_slot(int* character_slot)
	{
		STATS::STAT_GET_INT(RAGE_JOAAT("MPPLY_LAST_MP_CHAR"), character_slot, true);
	}

	/**
	 * @brief Gets MPx_ stat prefix. Where x is the current character slot.
	 * 
	 * @return MPx_ stat prefix.
	 */
	inline std::string get_mp_prefix()
	{
		int character_slot;
		get_active_character_slot(&character_slot);
		return "MP" + std::to_string(character_slot) + "_";
	}

	/**
	 * @brief Sets players GTA Online rank.
	 * Uses levels, not RP points.
	 * @note Maximum is 8000.
	 * 
	 * @param level Level to set.
	 */
	inline void set_player_level(int level)
	{
		STATS::STAT_SET_INT(rage::joaat(get_mp_prefix() + "CHAR_SET_RP_GIFT_ADMIN"), levels[level - 1], 0);
	}

	/**
	 * @brief Gives local ped max health and armor.
	 */
	inline void heal_player()
	{
		ENTITY::SET_ENTITY_HEALTH(self::ped, PED::GET_PED_MAX_HEALTH(self::ped), 0);
		PED::SET_PED_ARMOUR(self::ped, PLAYER::GET_PLAYER_MAX_ARMOUR(self::id));
	}

	/**
	 * @brief Gives the player maximum snacks and armor.
	 */
	inline void fill_inventory()
	{
		std::string mpPrefix = local_player::get_mp_prefix();
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "NO_BOUGHT_YUM_SNACKS"), 30, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "NO_BOUGHT_HEALTH_SNACKS"), 15, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "NO_BOUGHT_EPIC_SNACKS"), 5, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "NUMBER_OF_ORANGE_BOUGHT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "NUMBER_OF_BOURGE_BOUGHT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "CIGARETTES_BOUGHT"), 20, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "MP_CHAR_ARMOUR_1_COUNT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "MP_CHAR_ARMOUR_2_COUNT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "MP_CHAR_ARMOUR_3_COUNT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "MP_CHAR_ARMOUR_4_COUNT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "MP_CHAR_ARMOUR_5_COUNT"), 10, true);
		STATS::STAT_SET_INT(rage::joaat(mpPrefix + "MP_CHAR_ARMOUR_5_COUNT"), 10, true);
	}
}