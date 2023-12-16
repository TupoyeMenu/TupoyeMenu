/**
 * @file folder.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "folder.hpp"

#include "file_manager.hpp"

namespace big
{
	folder::folder(const std::filesystem::path& folder_path) :
	    m_folder_path(folder_path)
	{
	}

	file folder::get_file(std::filesystem::path file_path) const
	{
		if (file_path.is_absolute())
			throw std::invalid_argument("folder#get_file requires a relative path.");
		if (file_path.string().contains(".."))
			throw std::invalid_argument("Relative path traversal is not allowed, refrain from using \"..\" in file paths.");

		return file(m_folder_path / file_path);
	}

	folder folder::get_folder(std::filesystem::path folder_path) const
	{
		if (folder_path.is_absolute())
			throw std::invalid_argument("folder#get_folder requires a relative path.");
		if (folder_path.string().contains(".."))
			throw std::invalid_argument("Relative path traversal is not allowed, refrain from using \"..\" in file paths.");
		return folder(m_folder_path / folder_path);
	}

	const std::filesystem::path folder::get_path() const
	{
		return m_folder_path;
	}
}