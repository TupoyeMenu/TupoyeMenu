/**
 * @file update_presence_attribute.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef ENABLE_SOCIALCLUB
#include "hooking.hpp"
#include "services/player_database/player_database_service.hpp"

namespace big
{
	inline bool block_session_presence()
	{
		return g.protections.rid_join || (g_player_database_service && g_player_database_service->is_redirect_join_active());
	}

	bool hooks::update_presence_attribute_int(void* presence_data, int profile_index, char* attr, std::uint64_t value)
	{
		auto hash = rage::joaat(attr);
		if (block_session_presence() && (hash == RAGE_JOAAT("gstok") || hash == RAGE_JOAAT("gsid") || hash == RAGE_JOAAT("gstype") || hash == RAGE_JOAAT("gshost") || hash == RAGE_JOAAT("gsjoin")))
		{
			return true;
		}

		return g_hooking->get_original<hooks::update_presence_attribute_int>()(presence_data, profile_index, attr, value);
	}

	bool hooks::update_presence_attribute_string(void* presence_data, int profile_index, char* attr, char* value)
	{
		auto hash = rage::joaat(attr);
		if (block_session_presence() && hash == RAGE_JOAAT("gsinfo"))
		{
			return true;
		}

		// shouldn't have any side effects
		if (hash == RAGE_JOAAT("peeraddr"))
		{
			value = (char*)"";
		}

		return g_hooking->get_original<hooks::update_presence_attribute_string>()(presence_data, profile_index, attr, value);
	}
}
#endif // ENABLE_SOCIALCLUB