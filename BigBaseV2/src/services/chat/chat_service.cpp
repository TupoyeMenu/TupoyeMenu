#pragma warning( disable : 4566 ) // Unicode
#include "chat_service.hpp"
#include "network/CNetGamePlayer.hpp"
#include "gta/enums.hpp"
#include "pointers.hpp"
#include "natives.hpp"
#include "script.hpp"

namespace big
{
	chat_service::chat_service()
	{
		g_chat_service = this;
	}

	chat_service::~chat_service()
	{
		g_chat_service = nullptr;
	}

	bool chat_service::did_player_use_chat(Player player_id)
	{
		for (const auto& [player, is_team, is_spam, msg] : m_msgs)
		{
			if (player->m_player_id == player_id)
			{
				return true;
			}
		}
		return false;
	}

	void chat_service::add_msg(CNetGamePlayer* player, std::string_view msg, bool is_team, bool is_spam)
	{
		m_msgs.push_back({ player, is_team, is_spam, msg });
	}

	bool chat_service::is_spam(std::string_view text) // Skidded from https://github.com/HolyWurl/Block-GTASpamMessage/blob/master/Block-SpamMessage/dllmain.cpp
	{
		const char* words[] = { "外挂", "群", "刷", "VX", "微", "解锁", "辅助", "淘宝", "卡网", "科技", "成人", "少妇", "萝莉", "淫", "少女", "片", "官网", "www", "WWW", "xyz", "top", "cn", "QQ", "qq", "激情" };
		for (int i = 0; i < sizeof(words); i++)
		{
			if (strstr(text.data(), words[i]) != NULL)
				return true;
		}
		return false;
	}
	
	void chat_service::chat_menu()
	{
		while (g_running)
		{
			if (*g_pointers->m_is_session_started)
			{
				if (!g->window.chat && PAD::IS_CONTROL_JUST_PRESSED(0, (int)ControllerInputs::INPUT_MP_TEXT_CHAT_ALL))
				{
					HUD::MP_TEXT_CHAT_DISABLE(true);
					g->window.chat = true;
				}

				if (g->window.chat)
				{
					PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
					if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, (int)ControllerInputs::INPUT_FRONTEND_PAUSE_ALTERNATE))
					{
						PAD::DISABLE_ALL_CONTROL_ACTIONS(0); // TODO: Fix pause menu opening here.
						g->window.chat = false;
					}
				}
			}

			script::get_current()->yield();
		}
	}
}
