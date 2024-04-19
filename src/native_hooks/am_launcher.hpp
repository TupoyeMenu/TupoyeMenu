/**
 * @file am_launcher.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big
{
	namespace am_launcher
	{
		static const std::unordered_set<Hash> bad_script_hashes = {"ggsm_arcade"_J, "camhedz_arcade"_J, "wizard_arcade"_J, "puzzle"_J, "fm_intro"_J, "pilot_school_mp"_J, "golf_mp"_J, "tennis_network_mp"_J, "fm_race_controler"_J, "fm_horde_controler"_J, "fm_mission_controller"_J, "fm_mission_controller_2020"_J, "fm_impromptu_dm_controler"_J, "fm_deathmatch_controler"_J, "fm_bj_race_controler"_J, "fm_survival_controller"_J, "sctv"_J, "am_pi_menu"_J, "scroll_arcade_cabinet"_J, "grid_arcade_cabinet"_J, "degenatron_games"_J, "gunslinger_arcade"_J, "road_arcade"_J, "AM_MP_DRONE"_J};

		static void START_NEW_SCRIPT_WITH_ARGS(rage::scrNativeCallContext* src)
		{
			const char* const name = src->get_arg<const char*>(0);

			if (bad_script_hashes.contains(rage::joaat(name)))
			{
				src->set_return_value<int>(0);
				return;
			}

			Any* args            = src->get_arg<Any*>(1);
			const int argc       = src->get_arg<int>(2);
			const int stackSize  = src->get_arg<int>(3);

			src->set_return_value<int>(SYSTEM::START_NEW_SCRIPT_WITH_ARGS(name, args, argc, stackSize));
		}
	}
}
