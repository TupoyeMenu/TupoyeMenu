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
#include "file_manager/file.hpp"
#include "file_manager/folder.hpp"

namespace big
{
	class file_manager final
	{
	public:
		file_manager() = default;
		virtual ~file_manager() = default;
		file_manager(const file_manager&) = delete;
		file_manager(file_manager&&) noexcept = delete;
		file_manager& operator=(const file_manager&) = delete;
		file_manager& operator=(file_manager&&) noexcept = delete;

		bool init(const std::filesystem::path& base_dir);

		const std::filesystem::path& get_base_dir();

		file get_project_file(std::filesystem::path file_path);

		folder get_project_folder(std::filesystem::path folder_path);

		static std::filesystem::path ensure_file_can_be_created(const std::filesystem::path file_path);
		static std::filesystem::path ensure_folder_exists(const std::filesystem::path folder_path);

	private:
		std::filesystem::path m_base_dir;

	};
	inline auto g_file_manager = file_manager();
}