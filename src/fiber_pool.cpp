/**
 * @file fiber_pool.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "fiber_pool.hpp"

#include "common.hpp"
#include "script.hpp"
#include "script_mgr.hpp"
#include <script/tlsContext.hpp>

namespace big
{
	fiber_pool::fiber_pool(std::size_t num_fibers) :
	    m_num_fibers(num_fibers)
	{
		for (std::size_t i = 0; i < num_fibers; ++i)
		{
			g_script_mgr.add_script(std::make_unique<script>(&fiber_func));
		}

		g_fiber_pool = this;
	}

	fiber_pool::~fiber_pool()
	{
		g_fiber_pool = nullptr;
	}

	void fiber_pool::queue_job(std::function<void()> func)
	{
		if (func)
		{
			std::lock_guard lock(m_mutex);
			m_jobs.push(std::move(func));
		}
	}

	
	void fiber_pool::execute_on_game_thread(std::function<void()> func)
	{
		if (func)
		{
			if (rage::tlsContext::get()->m_script_thread && rage::tlsContext::get()->m_is_script_thread_active)
				func();
			else
				queue_job(func);
		}
	}

	void fiber_pool::fiber_tick()
	{
		std::unique_lock lock(m_mutex);
		if (!m_jobs.empty())
		{
			auto job = std::move(m_jobs.top());
			m_jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void fiber_pool::fiber_func()
	{
		while (true)
		{
			g_fiber_pool->fiber_tick();
			script::get_current()->yield();
		}
	}

	int fiber_pool::get_total_fibers()
	{
		return m_num_fibers;
	}

	int fiber_pool::get_used_fibers()
	{
		return m_jobs.size();
	}

	void fiber_pool::reset()
	{
		std::lock_guard lock(m_mutex);

		while (!m_jobs.empty())
			m_jobs.pop();
	}
}