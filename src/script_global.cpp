/**
 * @file script_global.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "script_global.hpp"

#include "common.hpp"
#include "pointers.hpp"

namespace big
{
	script_global::script_global(std::size_t index) :
	    m_index(index)
	{
	}

	script_global script_global::at(std::ptrdiff_t index)
	{
		return script_global(m_index + index);
	}

	script_global script_global::at(std::ptrdiff_t index, std::size_t size)
	{
		return script_global(m_index + 1 + (index * size));
	}

	void* script_global::get()
	{
		return g_pointers->m_gta.m_script_globals[m_index >> 0x12 & 0x3F] + (m_index & 0x3FFFF);
	}
}
