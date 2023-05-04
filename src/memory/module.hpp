/**
 * @file module.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "range.hpp"

namespace memory
{
	class module : public range
	{
	public:
		explicit module(const std::string_view name);

		/**
		 * @brief Get the export address of the current module given a symbol name
		 * 
		 * @param symbol_name 
		 * @return memory::handle 
		 */
		memory::handle get_export(std::string_view symbol_name);

		bool loaded() const;
		size_t size() const;

		/**
		 * @brief Waits till the given module is loaded.
		 * 
		 * @param time Time to wait before giving up.
		 * @return true 
		 * @return false 
		 */
		bool wait_for_module(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);

	protected:
		bool try_get_module();

	private:
		const std::string_view m_name;
		bool m_loaded;
	};
}
