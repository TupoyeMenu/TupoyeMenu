/**
 * @file pattern.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "fwddec.hpp"
#include "handle.hpp"

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace memory
{
	class pattern
	{
		friend pattern_batch;
		friend range;

	public:
		pattern(std::string_view ida_sig);

		inline pattern(const char* ida_sig) :
		    pattern(std::string_view(ida_sig))
		{
		}

		std::vector<std::optional<uint8_t>> m_bytes;
	};
}
