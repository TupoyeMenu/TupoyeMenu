/**
 * @file blip.hpp
 * @brief Map blip related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "gta/enums.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "pointers.hpp"
#include "ui/blip_t.hpp"
#include "ui/CBlipList.hpp"

namespace big::blip
{
	/**
	 * @brief Searches for blips based on the sprite and color provided.
	 * 
	 * @param location Blip location output.
	 * @param sprite Blip sprite to search for.
	 * @param color Color to search for, use -1 to ignore color search.
	 * @return True if blip exists, otherwise returns false.
	 */
	inline bool get_blip_location(Vector3& location, int sprite, int color = -1)
	{
		Blip blip;
		for (blip = HUD::GET_FIRST_BLIP_INFO_ID(sprite); HUD::DOES_BLIP_EXIST(blip) && color != -1 && HUD::GET_BLIP_COLOUR(blip) != color; blip = HUD::GET_NEXT_BLIP_INFO_ID(sprite))
			;

		if (!HUD::DOES_BLIP_EXIST(blip) || (color != -1 && HUD::GET_BLIP_COLOUR(blip) != color))
			return false;

		location = HUD::GET_BLIP_COORDS(blip);

		return true;
	}

	/**
	 * @brief Tries to find the objectibe marker and return it's location.
	 * 
	 * @param location Objective location output.
	 * @return True if it has faound the objective, otherwise returns false.
	 */
	inline bool get_objective_location(Vector3& location)
	{
		if (get_blip_location(location, (int)BlipIcons::Circle, (int)BlipColors::YellowMission))
			return true;
		if (get_blip_location(location, (int)BlipIcons::Circle, (int)BlipColors::YellowMission2))
			return true;
		if (get_blip_location(location, (int)BlipIcons::Circle, (int)BlipColors::Mission))
			return true;
		if (get_blip_location(location, (int)BlipIcons::RaceFinish, (int)BlipColors::None))
			return true;
		if (get_blip_location(location, (int)BlipIcons::Circle, (int)BlipColors::Green))
			return true;
		if (get_blip_location(location, (int)BlipIcons::Circle, (int)BlipColors::Blue))
			return true;
		if (get_blip_location(location, (int)BlipIcons::CrateDrop))
			return true;

		static const int blips[] = {1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288};
		for (const auto& blip : blips)
		{
			if (get_blip_location(location, blip, 5))
				return true;
		}

		return false;
	}

	inline rage::Blip_t* get_selected_blip()
	{
		for (int i = 0; i < 1500; i++)
		{

			auto blip = g_pointers->m_gta.m_blip_list->m_Blips[i].m_pBlip;
			if (blip && (blip->m_display_bits & BlipDisplayBits::BlipIsSelected))
			{
				return blip;
			}
		}
		return nullptr;
	}
}