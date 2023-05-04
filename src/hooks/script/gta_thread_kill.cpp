/**
 * @file gta_thread_kill.cpp
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
	rage::eThreadState hooks::gta_thread_kill(GtaThread* thread)
	{
		const auto result = g_hooking->get_original<gta_thread_kill>()(thread);

		if (g.notifications.gta_thread_kill.log)
			LOG(INFO) << "Script Thread '" << thread->m_name << "' terminated (" << thread->m_exit_message << ").";

		if (g.notifications.gta_thread_kill.notify)
			g_notification_service->push("Script Thread Termination",
			    std::format("Script Thread '{}' terminated.", thread->m_name));

		if (thread == g.m_hunt_the_beast_thread)
			g.m_hunt_the_beast_thread = nullptr;

		if (thread == g.m_dance_thread)
			g.m_dance_thread = nullptr;

		if (thread == g.m_mission_creator_thread)
			g.m_mission_creator_thread = nullptr;

		if (thread == g.m_modshop_thread)
			g.m_modshop_thread = nullptr;

		return result;
	}
}
