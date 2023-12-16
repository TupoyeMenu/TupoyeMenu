/**
 * @file folder.hpp
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
	class file;
	class file_manager;

	class folder
	{
	public:
		folder(const std::filesystem::path& folder_path = "");

		file get_file(std::filesystem::path file_path) const;
		folder get_folder(std::filesystem::path folder_path) const;
		const std::filesystem::path get_path() const;

	private:
		friend class file_manager;
		file_manager* m_file_manager;

		bool m_is_project_file;

		std::filesystem::path m_folder_path;
	};
}