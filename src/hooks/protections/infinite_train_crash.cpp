/**
 * @file infinite_train_crash.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "pointers.hpp"

namespace big
{
	void* hooks::infinite_train_crash(void* carriage)
	{
		void* current_carriage = carriage;
		int count              = 0;

		while (g_pointers->m_gta.m_get_next_carriage(current_carriage))
		{
			if (++count > 20)
				return nullptr;

			current_carriage = g_pointers->m_gta.m_get_next_carriage(current_carriage);
		}

		return current_carriage;
	}
}