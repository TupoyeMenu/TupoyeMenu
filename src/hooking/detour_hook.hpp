/**
 * @file detour_hook.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big
{
	class detour_hook
	{
	public:
		explicit detour_hook();
		explicit detour_hook(const std::string& name, void* detour);
		explicit detour_hook(const std::string& name, void* target, void* detour);
		~detour_hook() noexcept;

		detour_hook(detour_hook&& that)            = delete;
		detour_hook& operator=(detour_hook&& that) = delete;
		detour_hook(detour_hook const&)            = delete;
		detour_hook& operator=(detour_hook const&) = delete;

		void set_instance(const std::string& name, void* detour);
		void set_instance(const std::string& name, void* target, void* detour);

		void set_target_and_create_hook(void* target);

		void enable();
		void disable();

		template<typename T>
		T get_original()
		{
			return reinterpret_cast<T>(m_original);
		}

		void** get_original_ptr()
		{
			return &m_original;
		}

		void fix_hook_address();

	private:
		void create_hook();

		std::string m_name;
		void* m_original;
		void* m_target;
		void* m_detour;
	};
}
