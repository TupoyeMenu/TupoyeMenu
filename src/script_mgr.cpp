/**
 * @file script_mgr.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "script_mgr.hpp"

#include "common.hpp"
#include "gta/script_thread.hpp"
#include "gta_util.hpp"
#include "invoker.hpp"
#include "pointers.hpp"
#include "script/tlsContext.hpp"

namespace big
{
	void script_mgr::add_script(std::unique_ptr<script> script)
	{
		std::lock_guard lock(m_mutex);

		m_scripts.push_back(std::move(script));
	}

	void script_mgr::remove_all_scripts()
	{
		std::lock_guard lock(m_mutex);

		m_scripts.clear();
	}

	void script_mgr::tick()
	{
		gta_util::execute_as_script(RAGE_JOAAT("main_persistent"), std::mem_fn(&script_mgr::tick_internal), this);
	}

	void script_mgr::ensure_main_fiber()
	{
		ConvertThreadToFiber(nullptr);

		m_can_tick = true;
	}

#if defined (ENABLE_LUA)
	static void lua_manager_tick()
	{
		g_lua_manager->reload_changed_scripts();

		g_lua_manager->for_each_module([](const std::shared_ptr<lua_module>& module) {
			module->tick_scripts();
			module->cleanup_done_scripts();
		});
	}
#endif // ENABLE_LUA

	void script_mgr::tick_internal()
	{
		static bool ensure_it = (ensure_main_fiber(), true);

		std::lock_guard lock(m_mutex);

#if defined (ENABLE_LUA)
		lua_manager_tick();
#endif // ENABLE_LUA

		for (const auto& script : m_scripts)
		{
			if (script->is_enabled())
				script->tick();
		}
	}
}
