/**
 * @file script_patcher_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "memory/pattern.hpp"
#include "script_data.hpp"
#include "script_patch.hpp"

#include <script/scrProgram.hpp>

namespace big
{
	class script_patcher_service
	{
		std::vector<script_patch> m_script_patches;
		std::unordered_map<rage::joaat_t, std::unique_ptr<script_data>> m_script_data;
		script_data* get_data_for_script(rage::joaat_t script);
		bool does_script_have_patches(rage::joaat_t script);
		void create_data_for_script(rage::scrProgram* program);
		void update_all_patches_for_script(rage::joaat_t script);

	public:
		script_patcher_service();
		~script_patcher_service();

		void add_patch(script_patch&& patch);
		void on_script_load(rage::scrProgram* program);
		std::uint8_t** get_script_bytecode(rage::joaat_t script);
		void update();
	};

	inline script_patcher_service* g_script_patcher_service;
}