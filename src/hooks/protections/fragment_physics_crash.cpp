/**
 * @file fragment_physics_crash.cpp
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
	bool hooks::fragment_physics_crash(uintptr_t a1, uint32_t a2, uintptr_t a3, uintptr_t a4, uintptr_t a5)
	{
		if (auto ptr = *reinterpret_cast<uintptr_t*>(a5 + 0x70); ptr)
			if (auto ptr2 = *reinterpret_cast<uintptr_t*>(ptr + 8 * a2); !ptr2)
				return false;

		return g_hooking->get_original<hooks::fragment_physics_crash>()(a1, a2, a3, a4, a5);
	}

	bool hooks::fragment_physics_crash_2(float* a1, float* a2)
	{
		if (!a1 || !a2)
			return false;

		return g_hooking->get_original<hooks::fragment_physics_crash_2>()(a1, a2);
	}
}