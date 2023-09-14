/**
 * @file invalid_decal.cpp
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
	void hooks::invalid_decal(uintptr_t a1, int a2)
	{
		if (a1 && a2 == 2)
			//*(*(*(a1 + 0x48) + 0x30) + 0x2C8)
			if (const auto ptr = *reinterpret_cast<uintptr_t*>((a1 + 0x48)); ptr)
				if (const auto ptr2 = *reinterpret_cast<uintptr_t*>((ptr + 0x30)); ptr2)
					if (*reinterpret_cast<uintptr_t*>(ptr2 + 0x2C8) == 0)
						return;

		g_hooking->get_original<hooks::invalid_decal>()(a1, a2);
	}
}