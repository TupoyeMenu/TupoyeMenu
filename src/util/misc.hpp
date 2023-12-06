/**
 * @file misc.hpp
 * @brief Bitset functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big::misc
{
	template<typename T>
	inline void clear_bit(T* address, int pos)
	{
		*address &= ~(1 << pos);
	}
	
	template<typename T>
	inline void clear_bits(T* address, int bits)
	{
		*address &= ~(bits);
	}

	template<typename T>
	inline bool has_bit_set(T* address, int pos)
	{
		return *address & 1 << pos;
	}

	template<typename T>
	inline bool has_bits_set(T* address, T bits)
	{
		return (*address & bits) == bits;
	}

	template<typename T>
	inline void set_bit(T* address, int pos)
	{
		*address |= 1 << pos;
	}

	template<typename T>
	inline void set_bits(T* address, int bits)
	{
		*address |= bits;
	}

	inline Vector3 fvector3_to_Vector3(rage::fvector3 vector_in)
	{
		Vector3 vector_out = { vector_in.x, vector_in.y, vector_in.z };

		return vector_out;
	}
}
