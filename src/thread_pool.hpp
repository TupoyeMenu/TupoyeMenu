/**
 * @file thread_pool.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big
{
	struct thread_pool_job
	{
		std::function<void()> m_func;
		std::source_location m_source_location;
	};

	class thread_pool
	{
		std::atomic<bool> m_accept_jobs;
		std::condition_variable m_data_condition;

		std::stack<thread_pool_job> m_job_stack;
		std::mutex m_lock;
		std::vector<std::thread> m_thread_pool;

		std::thread m_managing_thread;

		std::atomic<size_t> m_available_thread_count;

	public:
		thread_pool();
		~thread_pool();

		void destroy();
		void push(std::function<void()> func, std::source_location location = std::source_location::current());

	private:
		void create();
		void done();
		void run();
	};

	inline thread_pool* g_thread_pool{};
}
