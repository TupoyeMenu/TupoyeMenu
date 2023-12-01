/**
 * @file handle.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace memory
{
	class handle
	{
	public:
		handle(void* ptr = nullptr);
		explicit handle(std::uintptr_t ptr);

		template<typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as() const;

		template<typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as() const;

		template<typename T>
		std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> as() const;

		template<typename T>
		handle add(T offset) const;

		template<typename T>
		handle sub(T offset) const;

		handle rip() const;

		explicit operator bool();

		friend bool operator==(handle a, handle b);
		friend bool operator!=(handle a, handle b);

	private:
		void* ptr;
	};

	inline handle::handle(void* ptr) :
	    ptr(ptr)
	{
	}

	inline handle::handle(std::uintptr_t ptr) :
	    ptr(reinterpret_cast<void*>(ptr))
	{
	}

	template<typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> handle::as() const
	{
		return reinterpret_cast<T>(ptr);
	}

	template<typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> handle::as() const
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(ptr);
	}

	template<typename T>
	inline std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> handle::as() const
	{
		return reinterpret_cast<std::uintptr_t>(ptr);
	}

	template<typename T>
	inline handle handle::add(T offset) const
	{
		return handle(as<std::uintptr_t>() + offset);
	}

	template<typename T>
	inline handle handle::sub(T offset) const
	{
		return handle(as<std::uintptr_t>() - offset);
	}

	inline handle handle::rip() const
	{
		return add(as<std::int32_t&>()).add(4);
	}

	inline bool operator==(handle a, handle b)
	{
		return a.ptr == b.ptr;
	}

	inline bool operator!=(handle a, handle b)
	{
		return a.ptr != b.ptr;
	}

	inline handle::operator bool()
	{
		return ptr != nullptr;
	}
}
