#include "hooking/hooking.hpp"
#include "pointers.hpp"
#include "services/player_database/player_database_service.hpp"

#include <network/CJoinRequestContext.hpp>
#include <network/CMsgJoinResponse.hpp>
#include <network/snSession.hpp>

namespace big
{
	/**
	 * @brief Checks if it's ok to let the player into the session.
	 * @param network Network.
	 * @param session Session that the player is trying to join.
	 * @param player_info Player that is joining.
	 * @param ctx Join context, just read the class.
	 * @param is_transition_session Is not actually a bool, seems to be some sort of class.
	 * @return bool 
	 */
	bool hooks::handle_join_request(Network* network, rage::snSession* session, rage::rlGamerInfo* player_info, CJoinRequestContext* ctx, BOOL is_transition_session)
	{
		LOG(VERBOSE) << player_info->m_name << " " << is_transition_session;
		if (auto player = g_player_database_service->get_player_by_rockstar_id(player_info->m_gamer_handle.m_rockstar_id); player)
		{
			if (player->block_join)
			{
				LOG(VERBOSE) << player->name << " " << &ctx->m_join_response_size;
				CMsgJoinResponse response{};
				response.m_status_code = player->block_join_reason;
				g_pointers->m_gta.m_write_join_response_data(&response, ctx->m_join_response_data, 512, &ctx->m_join_response_size);
				//g_notification_service.push("Block Join", std::format("Blocked {} from joining", player->name));
				return false;
			}
			if (player->force_allow_join)
			{
				LOG(VERBOSE) << player->name << " " << &ctx->m_join_response_size;
				CMsgJoinResponse response{};
				response.m_status_code = 0;
				g_pointers->m_gta.m_write_join_response_data(&response, ctx->m_join_response_data, 512, &ctx->m_join_response_size);
				//g_notification_service.push("Force Allow Join", std::format("Allowed {} to join", player->name));
				return true;
			}
		}

		return g_hooking->get_original<hooks::handle_join_request>()(network, session, player_info, ctx, is_transition_session);
	}
}
