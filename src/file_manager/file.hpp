/**
 * @file file.hpp
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
	class file_manager;

	class file
	{
	public:
		file(std::filesystem::path file_path);

		file copy(std::filesystem::path new_path);
		bool exists() const;
		const std::filesystem::path get_path() const;
		file move(std::filesystem::path new_path);

	protected:
		file(file_manager* file_manager, std::filesystem::path file_path);

	private:
		friend class file_manager;

		bool m_is_project_file;
		std::filesystem::path m_file_path;
	};
}