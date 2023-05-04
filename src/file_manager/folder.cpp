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
	folder::folder(file_manager* file_manager, std::filesystem::path file_path) :
	    folder(file_manager->get_base_dir() / file_path)
	{
		m_file_manager    = file_manager;
		m_is_project_file = true;
	}

	folder::folder(std::filesystem::path folder_path) :
	    m_folder_path(file_manager::ensure_folder_exists(folder_path))
	{
	}

	file folder::get_file(std::filesystem::path file_path) const
	{
		if (file_path.is_absolute())
			throw std::exception("folder#get_file requires a relative path.");

		return file(m_folder_path / file_path);
	}

	const std::filesystem::path folder::get_path() const
	{
		return m_folder_path;
	}
}