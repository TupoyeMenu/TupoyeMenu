#include "hooking.hpp"
#include "pointers.hpp"
#include "services/chat/chat_service.hpp"
#include "lua/lua_scripts.hpp"

namespace big
{
    __int64* hooks::chat_receive(__int64 a1, __int64 a2, __int64 sender, const char* msg, char is_team)
    {
        CNetGamePlayer* Player = g_pointers->m_get_net_player_from_unk(sender);

        if (Player != nullptr)
        {
            g_chat_service->add_msg(Player, msg, (bool)is_team);
            LOG(INFO) << Player->get_name() << ((bool)is_team ? " [LOCAL] " : " [ALL] ") << msg;

            lua_getglobal(lua_scripts::L, "hook");
            lua_getfield(lua_scripts::L, -1, "Call");
            lua_pushstring(lua_scripts::L, "on_chat_message");
            lua_pushstring(lua_scripts::L, msg);
            lua_pushstring(lua_scripts::L, Player->get_name());
            lua_pushinteger(lua_scripts::L, Player->get_net_data()->m_gamer_handle_2.m_rockstar_id);
            lua_pushstring(lua_scripts::L, &is_team);
            lua_pcall(lua_scripts::L, 5, 0, 0);
        }

        return g_hooking->m_chat_message_received_hook.get_original<decltype(&chat_receive)>()(a1, a2, sender, msg, is_team);
    }
}