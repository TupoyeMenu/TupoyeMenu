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
	void hooks::serialize_take_off_ped_variation_task(ClonedTakeOffPedVariationInfo* info, rage::CSyncDataBase* serializer)
	{
		g_hooking->get_original<hooks::serialize_take_off_ped_variation_task>()(info, serializer);
		if (info->m_prop_hash != 0 && info->m_variation_component == 5 && info->m_prop_hash != RAGE_JOAAT("p_parachute_s"))
		{
			// notify::crash_blocked(g.m_syncing_player, "invalid parachute"); false positives
			info->m_prop_hash = 0;
		}
	}
}
