/**
 * @file pools.hpp
 * @brief Pool Interator class to iterate over pools. Has just enough operators defined to be able to be used in a for loop, not suitable for any other iterating.
 * @note everything pasted from https://github.com/gta-chaos-mod/ChaosModV/blob/master/ChaosMod/Util/EntityIterator.h
 * Thanks to menyoo for most of these!!
 *
 * @copyright GNU General Public License Version 3.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>. 
 */

#pragma once
#include "natives.hpp"
#include "pointers.hpp"

template<typename T>
class pool_iterator
{
public:
	T* pool        = nullptr;
	uint32_t index = 0;

	explicit pool_iterator(T* pool, int32_t index = 0)
	{
		this->pool  = pool;
		this->index = index;
	}

	pool_iterator& operator++()
	{
		for (index++; index < pool->m_size; index++)
		{
			if (pool->is_valid(index))
			{
				return *this;
			}
		}

		index = pool->m_size;
		return *this;
	}

	rage::CEntity* operator*()
	{
		auto addr = pool->get_address(index);
		return (rage::CEntity*)addr;
	}

	bool operator!=(const pool_iterator& other) const
	{
		return this->index != other.index;
	}
};

template<typename T>
/**
 * @brief Common functions for VehiclePool and GenericPool
 */
class PoolUtils
{
public:
	inline auto to_array()
	{
		std::vector<Entity> arr;
		for (auto entity : *static_cast<T*>(this))
		{
			if (entity)
				arr.push_back(big::g_pointers->m_gta.m_ptr_to_handle(entity));
		}

		return arr;
	}

	inline auto to_int_array(int* arr, int max)
	{
		auto entities = to_array();

		if (entities.size() > max)
			entities.resize(max);

		for (int i = 0; i < entities.size(); ++i)
		{
			arr[i] = entities[i];
		}

		return entities.size();
	}

	auto begin()
	{
		return ++pool_iterator<T>(static_cast<T*>(this), -1);
	}

	auto end()
	{
		return ++pool_iterator<T>(static_cast<T*>(this), static_cast<T*>(this)->m_size);
	}
};

class VehiclePool : public PoolUtils<VehiclePool>
{
public:
	UINT64* m_pool_address;
	UINT32 m_size;
	char _Padding2[36];
	UINT32* m_bit_array;
	char _Padding3[40];
	UINT32 m_item_count;

	inline bool is_valid(UINT32 i)
	{
		return (m_bit_array[i >> 5] >> (i & 0x1F)) & 1;
	}

	inline UINT64 get_address(UINT32 i)
	{
		return m_pool_address[i];
	}
};

class GenericPool : public PoolUtils<GenericPool>
{
public:
	UINT64 m_pool_address; // 0x0
	BYTE* m_bit_array;     // 0x8
	UINT32 m_size;         // 0x10
	UINT32 m_item_size;    // 0x14
	UINT32 m_pad[2];       // 0x18
	UINT32 m_item_count;   // 0x20

	inline bool is_valid(UINT32 i)
	{
		return mask(i) != 0;
	}

	inline UINT64 get_address(UINT32 i)
	{
		return mask(i) & (m_pool_address + i * m_item_size);
	}

	inline int get_item_count()
	{
		return (4 * m_item_count) >> 2;
	}

private:
	inline long long mask(UINT32 i)
	{
		long long num1 = m_bit_array[i] & 0x80;
		return ~((num1 | -num1) >> 63);
	}
};
static_assert(offsetof(GenericPool, GenericPool::m_item_count) == 0x20);