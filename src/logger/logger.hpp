/**
 * @file logger.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "file_manager.hpp"

#include <AsyncLogger/Logger.hpp>
using namespace al;

namespace big
{
#define ADD_COLOR_TO_STREAM(color) "\x1b[" << int(color) << "m"
#define RESET_STREAM_COLOR "\x1b[" << int(LogColor::RESET) << "m"
#define HEX_TO_UPPER(value) "0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase

	enum class LogColor
	{
		RESET,
		WHITE   = 97,
		CYAN    = 36,
		MAGENTA = 35,
		BLUE    = 34,
		GREEN   = 32,
		YELLOW  = 33,
		RED     = 31,
		BLACK   = 30
	};

	class logger final
	{
	public:
		logger(std::string_view console_title, file file, bool attach_console = true);
		virtual ~logger();

		void initialize();
		void destroy();

		std::vector<LogMessagePtr> get_log_messages() { return m_log_messages; }
		void clear_log_messages();

	private:
		void create_backup();

		void open_outstreams();
		void close_outstreams();

		void format_console(const LogMessagePtr msg);
		void format_console_simple(const LogMessagePtr msg);
		void format_file(const LogMessagePtr msg);
		void format_log(const LogMessagePtr msg);

	private:
		bool m_attach_console;
		bool m_did_console_exist;

		void (logger::*m_console_logger)(const LogMessagePtr msg);

		std::string_view m_console_title;
		DWORD m_original_console_mode;
		HANDLE m_console_handle;

		std::ofstream m_console_out;
		std::ofstream m_file_out;
		std::vector<LogMessagePtr> m_log_messages;

		file m_file;
	};

	inline logger* g_log = nullptr;
}