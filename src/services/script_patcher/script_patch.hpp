/**
 * @file script_patch.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "memory/pattern.hpp"

namespace big
{
	struct script_data;

	class script_patch
	{
		rage::joaat_t m_script;
		const memory::pattern m_pattern;
		int32_t m_offset;
		std::vector<uint8_t> m_patch;
		std::vector<uint8_t> m_original;
		bool* m_bool;
		int32_t m_ip;

		static uint8_t* get_code_address(script_data* data, uint32_t index);
		static const std::optional<uint32_t> get_code_location_by_pattern(script_data* data, const memory::pattern& pattern);
		void enable(script_data* data);
		void disable(script_data* data);

	public:
		inline rage::joaat_t get_script()
		{
			return m_script;
		}

		script_patch(rage::joaat_t script, const memory::pattern pattern, int32_t offset, std::vector<uint8_t> patch, bool* enable_bool);
		void update(script_data* data);
	};
}