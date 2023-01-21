#include "hooking.hpp"
#include "gta_util.hpp"
#include "services/players/player_service.hpp"
#include "services/chat/chat_service.hpp"

#include "backend/command.hpp"
#include "backend/context/chat_command_context.hpp"

namespace big
{
	bool hooks::send_chat_message(void* team_mgr, rage::rlGamerInfo* local_gamer_info, char* message, bool is_team)
	{
		g_chat_service->add_msg(g_player_service->get_self()->get_net_game_player(), message, is_team, false);

		if (g.session.chat_commands && message[0] == g.session.chat_command_prefix)
			command::process(std::string(message + 1), std::make_shared<chat_command_context>(g_player_service->get_self()));

		return g_hooking->get_original<hooks::send_chat_message>()(team_mgr, local_gamer_info, message, is_team);
	}
}