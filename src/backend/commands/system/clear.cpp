#include "backend/command.hpp"

namespace big
{
	class clear : command
	{
		using command::command;

		virtual void execute(const command_arguments&, const std::shared_ptr<command_context> ctx) override
		{
			g_log.clear_log_messages();
		}
	};

	clear g_clear("clear", "Clear Console", "Clears all text in console", 0);
}
