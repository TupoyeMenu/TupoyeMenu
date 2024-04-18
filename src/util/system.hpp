/**
 * @file system.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "file_manager.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"

namespace big::system
{
	/**
	 * @brief Dumbs native entry points into entrypoints.txt
	 */
	inline void dump_entry_points()
	{
		// TODO!
		LOG(WARNING) << "Not implemented!";
#if 0
		DWORD64 base_address = memory::module("GTA5.exe").begin().as<DWORD64>();

		const auto file_path = g_file_manager.get_project_file("./entrypoints.txt");
		auto file            = std::ofstream(file_path.get_path(), std::ios::out | std::ios::trunc);

		for (auto& map : g_crossmap)
		{
			auto address = g_pointers->m_gta.m_get_native_handler(g_pointers->m_gta.m_native_registration_table, map.second);

			file << std::hex << std::uppercase << "0x" << map.first << " : GTA5.exe + 0x" << (DWORD64)address - base_address << std::endl;
		}

		file.close();
#endif
	}

	/**
	 * @brief Gets the address relative to GTA5.exe
	 */
	inline uintptr_t get_relative_address(void* ptr)
	{
		uintptr_t base_address = memory::module("GTA5.exe").begin().as<uintptr_t>();

		return (uintptr_t)ptr - base_address;
	}
}