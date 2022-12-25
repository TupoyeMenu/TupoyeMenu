#include "console_command_context.hpp"

namespace big
{
	player_ptr console_command_context::get_sender() const
	{
		return g_player_service->get_self();
	}

	CommandAccessLevel console_command_context::get_access_level() const
	{
		return CommandAccessLevel::ADMIN;
	}

	void console_command_context::report_output(const std::string& output) const
	{
		LOG(INFO) << output;
	}

	void console_command_context::report_error(const std::string& error) const
	{
		LOG(WARNING) << error;
	}
}