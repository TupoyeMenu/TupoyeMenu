/**
 * @file file_manager.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "common.hpp"
#include "file_manager/file.hpp"
#include "file_manager/folder.hpp"

namespace big
{
	class file_manager;
	inline file_manager* g_file_manager{};

	class file_manager final
	{
	public:
		file_manager(std::filesystem::path base_dir) :
		    m_base_dir(base_dir)
		{
			file_manager::ensure_folder_exists(m_base_dir);

			g_file_manager = this;
		}
		~file_manager()
		{
			g_file_manager = nullptr;
		}

		std::filesystem::path get_base_dir()
		{
			return m_base_dir;
		}

		file get_project_file(std::filesystem::path file_path)
		{
			if (file_path.is_absolute())
				throw std::exception("Project files are relative to the BaseDir, don't use absolute paths!");

			return file(this, file_path);
		}
		folder get_project_folder(std::filesystem::path folder_path)
		{
			if (folder_path.is_absolute())
				throw std::exception("Project folders are relative to the BaseDir, don't use absolute paths!");

			return folder(this, folder_path);
		}

		static std::filesystem::path ensure_file_can_be_created(const std::filesystem::path file_path)
		{
			file_manager::ensure_folder_exists(file_path.parent_path());

			return file_path;
		}
		static std::filesystem::path ensure_folder_exists(const std::filesystem::path folder_path)
		{
			bool create_path = !std::filesystem::exists(folder_path);

			if (!create_path && !std::filesystem::is_directory(folder_path))
			{
				std::filesystem::remove(folder_path);
				create_path = true;
			}
			if (create_path)
				std::filesystem::create_directory(folder_path);

			return folder_path;
		}

	private:
		std::filesystem::path m_base_dir;
	};
}