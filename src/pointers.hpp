/**
 * @file pointers.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "base/HashTable.hpp"
#include "function_types.hpp"
#include "gta/fwddec.hpp"
#include "gta_pointers.hpp"
#include "memory/module.hpp"
#include "socialclub/ScInfo.hpp"
#include "services/gta_data/cache_file.hpp"
#include "util/compile_time_helpers.hpp"
#include "gta_pointers.hpp"
#ifdef ENABLE_SOCIALCLUB
	#include "sc_pointers.hpp"
#endif // ENABLE_SOCIALCLUB

namespace big
{
	class pointers
	{
	private:
		static void get_gta_batch(memory::module region);
		static void get_sc_batch(memory::module region);

	public:
		explicit pointers();
		~pointers();

	public:
		HWND m_hwnd{};

		gta_pointers m_gta;

#ifdef ENABLE_SOCIALCLUB
		socialclub_pointers m_sc;
#endif // ENABLE_SOCIALCLUB		
	};

	inline pointers* g_pointers{};
}
