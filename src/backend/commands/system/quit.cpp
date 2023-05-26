#include "backend/command.hpp"

namespace big
{
	class quit : command
	{
		using command::command;

		virtual void execute(const std::vector<std::uint64_t>&, const std::shared_ptr<command_context> ctx)
		{
			g_running = false;
			exit(0);
		}
	};

	quit g_quit("quit", "Rage Quit", "We all have bad times sometimes. Close your GTA instant.", 0);
	quit g_exit("exit", "Exit", "We all have bad times sometimes. Close your GTA instant. Alias for quit.", 0);
}
