/**
 * @file script_local.hpp
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
	class script_local
	{
	public:
		explicit script_local(rage::scrThread* thread, std::size_t index);
		explicit script_local(PVOID stack, std::size_t index);
		explicit script_local(std::size_t index);

		script_local set(rage::scrThread* thread);
		script_local set(void* stack);

		script_local at(std::ptrdiff_t index);
		script_local at(std::ptrdiff_t index, std::size_t size);

		template<typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as()
		{
			return static_cast<T>(get());
		}

		template<typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
		{
			return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(get());
		}

	private:
		void* get();
		std::size_t m_index;
		PVOID m_stack;
	};
}
