#include "hooking/hooking.hpp"
#include "pointers.hpp"
#include "services/friends/friends_service.hpp"
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
		auto rockstar_id = player_info->m_gamer_handle.m_rockstar_id;
		auto is_friend   = friends_service::is_friend(rockstar_id);
		auto db_player   = g_player_database_service->get_player_by_rockstar_id(rockstar_id);
		auto block_join  = db_player && db_player->block_join;
		auto force_allow_join  = db_player && db_player->force_allow_join;
		auto is_trusted  = db_player && db_player->is_trusted;
		auto player_name = player_info->m_name;

		LOG(VERBOSE) << player_info->m_name << " " << is_transition_session;

		auto send_response = [ctx](int block_join_reason = 1) {
			CMsgJoinResponse response{};
			response.m_status_code = block_join_reason;
			g_pointers->m_gta.m_write_join_response_data(&response, ctx->m_join_response_data, 512, &ctx->m_join_response_size);
		};

		if (block_join)
		{
			send_response(db_player->block_join_reason);
			g_notification_service.push("Block Join", std::format("Blocked {} from joining", player_name));
			return false;
		}
		else if (force_allow_join)
		{
			send_response(0);
			g_notification_service.push("Force Allow Join", std::format("Allowed {} to join", player_name));
			return true;
		}
		else if (g.session.lock_session)
		{
			if ((is_friend && g.session.allow_friends_into_locked_session) || is_trusted)
			{
				g_notification_service.push_success("Lobby Lock", std::format("A friend or trusted player with the name of {} has been allowed to join the locked session", player_name));
			}
			else
			{
				send_response();
				g_notification_service.push_warning("Lobby Lock", std::format("A player with the name of {} has been denied entry", player_name));
				return false;
			}
		}

		return g_hooking->get_original<hooks::handle_join_request>()(network, session, player_info, ctx, is_transition_session);
	}
}
