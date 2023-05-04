/**
 * @file pe_image.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big::pe_utils
{
	typedef const IMAGE_NT_HEADERS64 NT64H;
	typedef std::vector<char> bufferVec;

	class pe_image
	{
	public:
		pe_image();
		~pe_image();

		bool load(const std::string& path);

		bool is_openvhook_compatible();

		bool patch_compatibility();

	private:
		uint64_t get_directory_address(int index);
		uint64_t rva_to_va(uint32_t rva) const;

		bool image_pe();

	private:
		std::string filePath;
		bufferVec fileBuffer;
		NT64H* ntHeader = nullptr;
	};

}
