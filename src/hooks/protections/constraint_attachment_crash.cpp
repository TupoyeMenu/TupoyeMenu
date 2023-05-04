/**
 * @file constraint_attachment_crash.cpp
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
	std::int64_t hooks::constraint_attachment_crash(std::uintptr_t a1)
	{
		if (!*reinterpret_cast<void**>(a1 + 0x38))
			return 0;

		return g_hooking->get_original<hooks::constraint_attachment_crash>()(a1);
	}
}