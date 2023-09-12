/**
 * @file serialize_take_off_ped_variation_task.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "util/notify.hpp"

#include <netsync/nodes/task/ClonedTakeOffPedVariationInfo.hpp>

namespace big
{
	constexpr uint32_t valid_parachute_models[] = {
	    RAGE_JOAAT("p_parachute_s"),
	    RAGE_JOAAT("vw_p_para_bag_vine_s"),
	    RAGE_JOAAT("reh_p_para_bag_reh_s_01a"),
	    RAGE_JOAAT("xm_prop_x17_scuba_tank"),
	    RAGE_JOAAT("lts_p_para_bag_pilot2_s"),
	    RAGE_JOAAT("lts_p_para_bag_lts_s"),
	    RAGE_JOAAT("p_para_bag_tr_s_01a"),
	    RAGE_JOAAT("p_para_bag_xmas_s"),
	};

	bool is_valid_parachute_model(rage::joaat_t hash)
	{
		for (auto& model : valid_parachute_models)
			if (model == hash)
				return true;

		return false;
	}

	void hooks::serialize_take_off_ped_variation_task(ClonedTakeOffPedVariationInfo* info, rage::CSyncDataBase* serializer)
	{
		g_hooking->get_original<hooks::serialize_take_off_ped_variation_task>()(info, serializer);
		if (!is_valid_parachute_model(info->m_prop_hash))
		{
			notify::crash_blocked(g.m_syncing_player, "invalid parachute model");
			info->m_prop_hash = 0;
		}
	}
}
