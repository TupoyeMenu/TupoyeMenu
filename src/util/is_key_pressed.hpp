/**
 * @file is_key_pressed.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "pointers.hpp"

namespace big
{
	/**
	 * @brief Uses GetAsyncKeyState to check key state.
	 * 
	 * @note You should be using wndproc most of the time. See big::renderer::add_wndproc_callback(big::wndproc_callback).
	 * 
	 * @param key Key to check.
	 * @return True if key is pressed and you are focused on the game window.
	 */
	inline bool is_key_pressed(uint16_t key)
	{
		if (GetForegroundWindow() == g_pointers->m_hwnd)
		{
			if (GetAsyncKeyState(key) & 0x8000)
			{
				return true;
			}
		}
		return false;
	}
}