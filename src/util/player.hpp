#pragma once
#include "gta/joaat.hpp"
#include "natives.hpp"
#include "core/scr_globals.hpp"
#include "script_global.hpp"

namespace big::util::player
{
	template <typename T> // Skidded from gir489
	inline  T get_player_stat(Player player, ePlayerStatType type)
	{
		return *script_global(scr_globals::player_stats_global).at(player, 862).at(205).at(type).as<T*>();
	}

	inline float get_player_speed(Player player)
	{
		Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		return ENTITY::GET_ENTITY_SPEED(ped);
	}
}
