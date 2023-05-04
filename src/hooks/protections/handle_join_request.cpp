/**
 * @file handle_join_request.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "pointers.hpp"
#include "services/player_database/player_database_service.hpp"

#include <network/CJoinRequestContext.hpp>
#include <network/CMsgJoinResponse.hpp>
#include <network/snSession.hpp>

namespace big
{
	bool hooks::handle_join_request(Network* network, rage::snSession* session, rage::rlGamerInfo* player_info, CJoinRequestContext* ctx, BOOL is_transition_session)
	{
		if (auto player = g_player_database_service->get_player_by_rockstar_id(player_info->m_gamer_handle.m_rockstar_id); player)
		{
			if (player->block_join)
			{
				CMsgJoinResponse response{};
				response.m_status_code = player->block_join_reason;
				g_pointers->m_gta.m_write_join_response_data(&response, ctx->m_join_response_data, 512, &ctx->m_join_response_size);
				g_notification_service->push("Block Join",
				    std::format("Blocked {} from joining", player->name));
				return false;
			}
			if (player->force_allow_join)
			{
				CMsgJoinResponse response{};
				response.m_status_code = 0;
				g_pointers->m_gta.m_write_join_response_data(&response, ctx->m_join_response_data, 512, &ctx->m_join_response_size);
				g_notification_service->push("Force Allow Join", std::format("Allowed {} to join", player->name));
				return true;
			}
		}

		return g_hooking->get_original<hooks::handle_join_request>()(network, session, player_info, ctx, is_transition_session);
	}
}