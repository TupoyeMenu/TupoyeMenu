#include "backend/command.hpp"

namespace big
{
	class help : command
	{
		using command::command;

		virtual std::optional<command_arguments> parse_args(const std::vector<std::string>& args, const std::shared_ptr<command_context> ctx) override
		{
			auto hash = rage::joaat(args[0]);
			return std::vector<std::uint64_t>{hash};
		}

		virtual void execute(const command_arguments& args, const std::shared_ptr<command_context> ctx) override
		{
			auto cmd = command::get(args.get(0));
			
			if(cmd)
		  		ctx->report_output(std::format("Name: {} \nDescription: {} \nNum Args: {}", cmd->get_label(), cmd->get_description(), cmd->get_num_args() ? cmd->get_num_args().value() : 0));
			else
				ctx->report_error("Failed to find this command");
		}
	};

	help g_help("help", "Help", "Gets command description", 1);
}
