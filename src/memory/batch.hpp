/**
 * @file batch.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "pattern.hpp"

#include <functional>
#include <vector>

namespace memory
{
	class batch
	{
	public:
		explicit batch()  = default;
		~batch() noexcept = default;

		void add(std::string name, pattern pattern, std::function<void(memory::handle)> callback);
		bool run(range region);

		struct entry
		{
			std::string m_name;
			pattern m_pattern;
			std::function<void(memory::handle)> m_callback;

			explicit entry(std::string name, pattern pattern, std::function<void(memory::handle)> callback) :
			    m_name(std::move(name)),
			    m_pattern(std::move(pattern)),
			    m_callback(std::move(callback))
			{
			}
		};

	private:
		std::vector<entry> m_entries;
	};
}