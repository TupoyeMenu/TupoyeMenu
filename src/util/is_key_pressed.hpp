/**
 * @file is_key_pressed.hpp
 * 
 * @copyright GNU General Public License Version 2.
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
	inline bool is_key_pressed(std::uint16_t key)
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