#include "hooking.hpp"
#include "pointers.hpp"
#include "services/chat/chat_service.hpp"

namespace big
{
    __int64* hooks::chat_receive(__int64 a1, __int64 a2, __int64 sender, const char* msg, bool is_team)
    {
        CNetGamePlayer* net_player = g_pointers->m_get_net_player_from_unk(sender);

        if (net_player != nullptr)
        {
            bool is_spam = g_chat_service->is_spam(msg);
            g_chat_service->add_msg(net_player, msg, is_team, is_spam);

            if(is_spam)
                return nullptr; //Blocks the message from appearing in normal chat

            LOG(INFO) << net_player->get_name() << (is_team ? " [LOCAL] " : " [ALL] ") << msg;
        }

        return g_hooking->m_chat_message_received_hook.get_original<decltype(&chat_receive)>()(a1, a2, sender, msg, is_team);
    }
}
