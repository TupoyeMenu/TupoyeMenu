/**
 * @file script_data.hpp
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
	class script_data
	{
		uint32_t m_num_pages;

	public:
		uint32_t m_code_size;
		uint8_t** m_bytecode;

		script_data(uint32_t code_size, uint8_t** bytecode, uint32_t num_pages) :
		    m_code_size(code_size),
		    m_bytecode(bytecode),
		    m_num_pages(num_pages)
		{
		}

		script_data(const script_data& that)            = delete;
		script_data& operator=(const script_data& that) = delete;

		~script_data()
		{
			for (auto i = 0u; i < m_num_pages; i++)
			{
				delete[] m_bytecode[i];
			}

			delete[] m_bytecode;
			m_bytecode = nullptr;
		}
	};
}