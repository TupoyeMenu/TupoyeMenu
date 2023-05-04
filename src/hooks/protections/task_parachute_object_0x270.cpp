/**
 * @file task_parachute_object_0x270.cpp
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
	uint64_t hooks::task_parachute_object_0x270(uint64_t _this, int a2, int a3)
	{
		if (a2 == 1 && a3 == 1)// enter crash func
		{
			if (auto ptr = *(uint64_t*)(_this + 16))
				if (auto ptr2 = *(uint64_t*)(ptr + 80))
					if (auto ptr3 = *(uint64_t*)(ptr2 + 64))
						return g_hooking->get_original<hooks::task_parachute_object_0x270>()(_this, a2, a3);
			return 0;
		}
		return g_hooking->get_original<hooks::task_parachute_object_0x270>()(_this, a2, a3);
	}
}