/**
 * @file custom_text_callbacks.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "custom_text_callbacks.hpp"
#include "hooking/hooking.hpp"
#include "pointers.hpp"

namespace big
{
	const char* respawn_label_callback(const char* label)
	{
		if (g.self.god_mode)
			return "~r~Dying with god mode, how?";

		return nullptr;
	}

	const char* do_ceo_name_resize(const char* label)
	{
		auto original = g_hooking->get_original<hooks::get_label_text>()(g_pointers->m_gta.m_ctext_file_ptr, label);
		if (auto pos = strstr((char*)original, "15"))
		{
			pos[0] = '4';
			pos[1] = '1';
		}
		return original;
	}
}
