/**
 * @file exception_handler.hpp
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
	class exception_handler final
	{
	public:
		exception_handler();
		virtual ~exception_handler();

	private:
		void* m_exception_handler;
		uint32_t m_old_error_mode;
	};

	extern LONG vectored_exception_handler(EXCEPTION_POINTERS* exception_info);
}