/**
 * @file serialize_stats.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "network/Network.hpp"

#include <stats/CStatsSerializationContext.hpp>

namespace big
{
	void hooks::serialize_stats(CStatsSerializationContext* context, rage::joaat_t* stats, uint32_t stat_count)
	{
		g_hooking->get_original<hooks::serialize_stats>()(context, stats, stat_count);

		// variable length array so we can't iterate it directly. Looks ugly but should work
		uint32_t* data = reinterpret_cast<uint32_t*>(context->m_entries);

		for (int i = 0; i < context->m_size / 4; i++)
		{
			CStatSerializationEntry* entry = reinterpret_cast<CStatSerializationEntry*>(&data[i]);

			if (g.spoofing.spoof_rank && (entry->m_hash == RAGE_JOAAT("MP0_CHAR_RANK_FM") || entry->m_hash == RAGE_JOAAT("MP1_CHAR_RANK_FM")))
			{
				entry->m_int_value = g.spoofing.rank;
			}

			if (g.spoofing.spoof_bad_sport && entry->m_hash == RAGE_JOAAT("MPPLY_OVERALL_BADSPORT"))
			{
				switch (g.spoofing.badsport_type)
				{
				case 0: entry->m_float_value = 0.0f; break;
				case 1: entry->m_float_value = 46.0f; break;
				case 2: entry->m_float_value = 1000.0f; break;
				}
			}

			if (g.spoofing.spoof_kd_ratio)
			{
				if (entry->m_hash == RAGE_JOAAT("MPPLY_KILLS_PLAYERS"))
					entry->m_int_value = g.spoofing.kd_ratio;// TODO precision loss

				if (entry->m_hash == RAGE_JOAAT("MPPLY_DEATHS_PLAYER"))
					entry->m_int_value = 1;

				if (entry->m_hash == RAGE_JOAAT("MPPLY_DEATHS_PLAYER_SUICIDE"))
					entry->m_int_value = 0;
			}
		}
	}
}