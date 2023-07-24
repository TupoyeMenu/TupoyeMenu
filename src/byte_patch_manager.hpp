/**
 * @file byte_patch_manager.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

extern "C" uint64_t g_sound_overload_ret_addr;

namespace big
{
	class byte_patch_manager
	{
	public:
		byte_patch_manager();
		~byte_patch_manager();
	};

	inline byte_patch_manager* g_byte_patch_manager;
}
