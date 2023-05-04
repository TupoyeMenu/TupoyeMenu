/**
 * @file sort_session_details.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"

#include <network/Network.hpp>

namespace big
{
	// true => e1 > e2
	// false => e1 < e2
	bool hooks::sort_session_details(SessionSortEntry* e1, SessionSortEntry* e2)
	{
		if (g.session.player_magnet_enabled)
		{
			return std::abs((int)e1->m_session_detail->m_player_count - g.session.player_magnet_count)
			    < std::abs((int)e2->m_session_detail->m_player_count - g.session.player_magnet_count);
		}
		else
		{
			return g_hooking->get_original<hooks::sort_session_details>()(e1, e2);
		}
	}
}