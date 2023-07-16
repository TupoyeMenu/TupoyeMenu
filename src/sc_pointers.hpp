/**
 * @file sc_pointers.hpp
 * @brief SocialClub.dll pointer declarations.
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
	// needed for serialization of the pointers cache
#pragma pack(push, 1)
	struct socialclub_pointers
	{
		functions::update_presence_attribute_int m_update_presence_attribute_int;
		functions::update_presence_attribute_string m_update_presence_attribute_string;

		functions::start_get_presence_attributes m_start_get_presence_attributes;
		PVOID m_read_attribute_patch;
		PVOID m_read_attribute_patch_2;
	};
#pragma pack(pop)

	static_assert(sizeof(socialclub_pointers) % 8 == 0, "Pointers are not properly aligned");
}
