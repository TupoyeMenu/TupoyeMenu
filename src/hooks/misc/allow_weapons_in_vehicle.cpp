/**
 * @file allow_weapons_in_vehicle.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"

namespace big
{
	bool hooks::allow_weapons_in_vehicle(int64_t unk, int weaponinfo_group)
	{
		if (g.vehicle.unlimited_weapons)
		{
			if (weaponinfo_group == RAGE_JOAAT("GROUP_THROWN")) // Because the thrower can be used without bypassing, we let the game decide the return value, since this function has multiple xrefs, it may be necessary to return 0 at some point
				return g_hooking->get_original<allow_weapons_in_vehicle>()(unk, weaponinfo_group);
			return true;
		}
		return g_hooking->get_original<allow_weapons_in_vehicle>()(unk, weaponinfo_group);
	}
}