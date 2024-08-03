/**
 * @file blip.hpp
 * @brief Map blip related functions.
 */

#pragma once
#include "gta/enums.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
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
	bool get_blip_location(Vector3& location, int sprite, int color = -1);

	bool get_objective_location_iteration(Vector3& location, const std::unordered_set<BlipIcons> sprites, const std::unordered_set<BlipColors> blip_colors);

	bool get_blip_location_from_offset(Vector3& location, int sprite);

	/**
	 * @brief Tries to find the objectibe marker and return it's location.
	 * 
	 * @param location Objective location output.
	 * @return True if it has faound the objective, otherwise returns false.
	 */
	bool get_objective_location(Vector3& location);

	rage::CBlip* get_selected_blip();
}