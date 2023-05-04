/**
 * @file gta_pointers_layout_info.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "pointers.hpp"

namespace big
{
	struct gta_pointers_layout_info
	{
		// get the beginning and the end of what we need to save / load
		inline static constexpr size_t offset_of_cache_begin_field = offsetof(big::pointers, m_gta);
		inline static constexpr size_t offset_of_cache_end_field = offsetof(big::pointers, m_gta) + sizeof(gta_pointers);
		inline static constexpr size_t field_count = (offset_of_cache_end_field - offset_of_cache_begin_field) / sizeof(void*);
	};
}
