/**
 * @file fiber_pool.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "common.hpp"

namespace big
{
	class fiber_pool
	{
	public:
		explicit fiber_pool(std::size_t num_fibers);
		~fiber_pool();

		void queue_job(std::function<void()> func);

		void fiber_tick();
		static void fiber_func();

		int get_total_fibers();
		int get_used_fibers();

		void reset();

	private:
		std::recursive_mutex m_mutex;
		std::stack<std::function<void()>> m_jobs;
		int m_num_fibers;
	};

	inline fiber_pool* g_fiber_pool{};
}
