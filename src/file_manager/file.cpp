/**
 * @file file.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "file.hpp"

#include "common.hpp"
#include "file_manager.hpp"

namespace big
{
	file::file(const std::filesystem::path& file_path) :
	    m_file_path(file_path)
	{
	}

	void file::operator=(const file& other)
	{
		m_file_path = other.m_file_path;
	}

	file::operator std::filesystem::path()
	{
		return m_file_path;
	}

	file::operator std::filesystem::path&()
	{
		return m_file_path;
	}

	bool file::exists() const
	{
		return std::filesystem::exists(m_file_path);
	}

	const std::filesystem::path file::get_path() const
	{
		return m_file_path;
	}

	file file::move(std::filesystem::path new_path)
	{
		if (new_path.is_relative())
			new_path = m_file_path.parent_path() / new_path;

		file_manager::ensure_file_can_be_created(new_path);

		if (std::filesystem::exists(m_file_path))
			std::filesystem::rename(m_file_path, new_path);

		return {new_path};
	}
}