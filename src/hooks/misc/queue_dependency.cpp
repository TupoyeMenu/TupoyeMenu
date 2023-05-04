/**
 * @file queue_dependency.cpp
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
	void hooks::queue_dependency(void* dependency)
	{
		if (dependency == g_pointers->m_gta.m_interval_check_func)
		{
			//LOG(INFO) << "Prevent attempt to queue a report to R*.";
			return;
		}

		return g_hooking->get_original<hooks::queue_dependency>()(dependency);
	}
}