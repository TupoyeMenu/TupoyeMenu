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

		void add_script(std::unique_ptr<script> script);
		void remove_all_scripts();

		script_list& scripts();

		void tick();

	private:
		void tick_internal();

	private:
		std::recursive_mutex m_mutex;
		script_list m_scripts;
	};

	inline script_mgr g_script_mgr;
}