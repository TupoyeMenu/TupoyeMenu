/**
 * @file shv_runner.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef ENABLE_ASI_LOADER
#include "shv_runner.hpp"

#include "common.hpp"
#include "natives.hpp"
#include "script.hpp"

namespace big
{
	void shv_runner::run_tick()
	{
		for (auto func : scripts)
		{
			func.second();
		}
	}

	void shv_runner::script_func()
	{
		while (g_running)
		{
			run_tick();
			script::get_current()->yield();
		}
	}

	void shv_runner::shutdown()
	{
		for (auto& iter : big::shv_runner::scripts)
		{
			big::shv_runner::scripts.erase(iter.first);
			FreeLibrary(iter.first);
			CloseHandle(iter.first);
		}
	}
}
#endif // ENABLE_ASI_LOADER
