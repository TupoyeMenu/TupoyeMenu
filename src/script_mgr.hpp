/**
 * @file script_mgr.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "common.hpp"
#include "script.hpp"

namespace big
{
	using script_list = std::vector<std::unique_ptr<script>>;

	class script_mgr
	{
	public:
		explicit script_mgr() = default;
		~script_mgr()         = default;

		script* add_script(std::unique_ptr<script> script);
		void remove_script(script* script);
		void remove_all_scripts();

		void add_on_script_batch_removed(std::function<void()> f);

		inline void for_each_script(auto func)
		{
			std::lock_guard lock(m_mutex);

			for (const auto& script : m_scripts)
			{
				func(script);
			}
		}

		void tick();

		[[nodiscard]] inline bool can_tick() const
		{
			return m_can_tick;
		}

	private:
		void ensure_main_fiber();
		void tick_internal();

	private:
		std::recursive_mutex m_mutex;
		script_list m_scripts;
		script_list m_scripts_to_add;
		std::queue<std::function<void()>> m_on_script_batch_removed;

		bool m_can_tick = false;
	};

	inline script_mgr g_script_mgr;
}