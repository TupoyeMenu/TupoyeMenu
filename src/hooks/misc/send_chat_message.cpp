/**
 * @file send_chat_message.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "backend/command.hpp"
#include "backend/context/chat_command_context.hpp"
#include "gta/net_game_event.hpp"
#include "gta_util.hpp"
#include "hooking/hooking.hpp"
#include "packet.hpp"
#include "services/players/player_service.hpp"
#include "util/chat.hpp"

#if defined(ENABLE_LUA)
#include "lua/lua_manager.hpp"
#endif // ENABLE_LUA

namespace big
{
	bool hooks::send_chat_message(void* team_mgr, rage::rlGamerInfo* local_gamer_info, char* message, bool is_team)
	{
		if (g.session.chat_commands && message[0] == g.session.chat_command_prefix)
			command::process(std::string(message + 1), std::make_shared<chat_command_context>(g_player_service->get_self()));
#if defined(ENABLE_LUA)
		else
			g_lua_manager->trigger_event<menu_event::ChatMessageReceived>(self::id, message);
#endif // ENABLE_LUA

		chat::send_message(message, nullptr, false, is_team);

		if (g.session.log_chat_messages)
			chat::log_chat(message, g_player_service->get_self(), SpamReason::NOT_A_SPAMMER, is_team);

		return true;
	}
}
