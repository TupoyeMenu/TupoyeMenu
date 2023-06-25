#include "backend/command.hpp"

#include "backend/looped_command.hpp" 

namespace big
{
	class unload : command
	{
		using command::command;

		virtual void execute(const std::vector<std::uint64_t>&, const std::shared_ptr<command_context> ctx)
		{
            for (auto& command : g_looped_commands)
				if (command->is_enabled())
					command->on_disable();
            g_running = false;
		}
	};

	unload g_unload("unload", "Unload Menu", "Unloads TupoyeMenu", 0);
}
