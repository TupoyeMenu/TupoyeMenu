/**
 * @file script_local.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "script_local.hpp"

#include "common.hpp"
#include "gta/script_thread.hpp"
#include "pointers.hpp"

namespace big
{
	script_local::script_local(rage::scrThread* thread, std::size_t index) :
	    m_index(index),
	    m_stack(thread->m_stack)
	{
	}
	script_local::script_local(PVOID stack, std::size_t index) :
	    m_index(index),
	    m_stack(stack)
	{
	}

	script_local script_local::at(std::ptrdiff_t index)
	{
		return script_local(m_stack, m_index + index);
	}

	script_local script_local::at(std::ptrdiff_t index, std::size_t size)
	{
		return script_local(m_stack, m_index + 1 + (index * size));
	}

	void* script_local::get()
	{
		return reinterpret_cast<uintptr_t*>((uintptr_t)m_stack + (m_index * sizeof(uintptr_t)));
	}
}
