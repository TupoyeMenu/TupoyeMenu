#pragma once


namespace lua_scripts
{
	static int lua_send_message(lua_State* L)
	{
		if (lua_isstring(L, 1) && lua_isboolean(L, 2))
		{
			lua_pushboolean(L, big::g_pointers->m_send_chat_message(*big::g_pointers->m_send_chat_ptr + 0x78, (__int64)big::g_local_player->m_player_info + 0x88, lua_tostring(L, 1), lua_toboolean(L, 2)));
		}
		return 1;
	}

	int luaopen_chat(lua_State* L)
	{
		static const struct luaL_Reg chat[] =
		{
			//{"on_message", lua_on_message}, // chat.on_message(function(packet_sender, message_sender, text, team_chat) end)
			{"send_message", lua_send_message}, // chat.send_message(char text, bool local)
			{NULL, NULL}
		};

		luaL_newlib(L, chat);
		return 1;
	}
}
