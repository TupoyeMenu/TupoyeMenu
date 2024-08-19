#pragma once

#include "core/scr_globals.hpp"
#include "gta/joaat.hpp"
#include "natives.hpp"
#include "pointers.hpp"

#include <script/globals/GPBD_FM.hpp>

namespace big::transition
{
	inline bool is_tutorial_active()
	{
		int character_slot;
		STATS::STAT_GET_INT("MPPLY_LAST_MP_CHAR"_J, &character_slot, true);

		Hash FM_TRIGTUT_DONE    = STATS::_GET_STAT_HASH_FOR_CHARACTER_STAT(2, 120, character_slot);
		Hash FM_RACETUT_DONE    = STATS::_GET_STAT_HASH_FOR_CHARACTER_STAT(2, 124, character_slot);
		Hash FM_INTRO_CUT_DONE  = STATS::_GET_STAT_HASH_FOR_CHARACTER_STAT(2, 115, character_slot);
		Hash FM_INTRO_MISS_DONE = STATS::_GET_STAT_HASH_FOR_CHARACTER_STAT(2, 119, character_slot);

		BOOL trigtut_done, racetut_done, intro_cut_done, intro_miss_done;
		STATS::STAT_GET_BOOL(FM_TRIGTUT_DONE, &trigtut_done, -1);
		STATS::STAT_GET_BOOL(FM_RACETUT_DONE, &racetut_done, -1);
		STATS::STAT_GET_BOOL(FM_INTRO_CUT_DONE, &intro_cut_done, -1);
		STATS::STAT_GET_BOOL(FM_INTRO_MISS_DONE, &intro_miss_done, -1);

		return !trigtut_done || !racetut_done || !intro_cut_done || !intro_miss_done || NETWORK::NETWORK_IS_IN_TUTORIAL_SESSION();
	}

	inline bool is_on_mission()
	{
		if (NETWORK::NETWORK_IS_ACTIVITY_SESSION())
			return true;

		int activity = scr_globals::gpbd_fm_1.as<GPBD_FM*>()->Entries[self::id].CurrentActivity;
		switch (activity)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 8:
		case 122: return true;
		}
		return false;
	}

	inline bool should_use_seamless_join()
	{
		return g.session.seamless_join && (!is_tutorial_active() && !is_on_mission() || !*g_pointers->m_gta.m_is_session_started);
	}
}