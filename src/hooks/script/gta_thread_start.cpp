/**
 * @file gta_thread_start.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "native_hooks/native_hooks.hpp"

namespace big
{
	GtaThread* hooks::gta_thread_start(unsigned int** a1, unsigned int a2)
	{
		GtaThread* new_thread = g_hooking->get_original<hooks::gta_thread_start>()(a1, a2);

		if (const char* name = new_thread->m_name; strlen(name) > 0)
		{
			if (g.notifications.gta_thread_kill.log)
				LOG(INFO) << "Script Thread '" << name << "' started.";
			if (g.notifications.gta_thread_kill.notify)
				g_notification_service->push("Script Thread Startup", std::format("Script Thread '{}' started.", name));
		}

		return new_thread;
	}
}
