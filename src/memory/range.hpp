/**
 * @file range.hpp
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

#include <vector>

namespace memory
{
	class range
	{
	public:
		range(handle base, std::size_t size);

		handle begin() const;
		handle end() const;
		std::size_t size() const;

		bool contains(handle h) const;

		handle scan(pattern const& sig) const;
		std::vector<handle> scan_all(pattern const& sig) const;

	protected:
		handle m_base;
		std::size_t m_size;
	};
}
