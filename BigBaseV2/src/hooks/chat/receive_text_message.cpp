#include "hooking.hpp"
#include "pointers.hpp"
#include "fiber_pool.hpp"
#include "script.hpp"
#include "chat.hpp"
#include "lua/lua_scripts.hpp"

namespace big
{
    __int64* hooks::chat_receive(__int64 chat_pointer, __int64 unk2, __int64 peerId, const char* msg, char IsTeam) 
    {
        CNetGamePlayer* PlayerID = (CNetGamePlayer*)(g_pointers->m_chat_player_id(peerId, &unk2, peerId));

        //Add the message to the list
        //log_msg((char*)msg, PlayerID->get_net_data()->m_name);

        LOG(INFO) << PlayerID->get_net_data()->m_rockstar_id2 << ", " << PlayerID->get_net_data()->m_name << ", " << msg << ", " << IsTeam;

        //Lua
        lua_getglobal(lua_scripts::L, "hook");
        lua_getfield(lua_scripts::L, -1, "Call");
        lua_pushstring(lua_scripts::L, "on_chat_message");
        lua_pushstring(lua_scripts::L, msg);
        lua_pushstring(lua_scripts::L, PlayerID->get_net_data()->m_name);
        lua_pushinteger(lua_scripts::L, PlayerID->get_net_data()->m_rockstar_id2);
        lua_pushstring(lua_scripts::L, &IsTeam);
        lua_pcall(lua_scripts::L, 5, 0, 0);
        //

        return g_hooking->m_chat_message_received_hook.get_original<decltype(&chat_receive)>()(chat_pointer, unk2, peerId, msg, IsTeam);
    }
}
