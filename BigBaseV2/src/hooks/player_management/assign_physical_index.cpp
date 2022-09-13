#include "hooking.hpp"
#include "services/players/player_service.hpp"
#include "services/player_database/player_database_service.hpp"
#include "util/notify.hpp"

namespace big
{
	void* hooks::assign_physical_index(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, uint8_t new_index)
	{
		auto returnResult = g_hooking->m_assign_physical_index_hook.get_original<decltype(&hooks::assign_physical_index)>()(netPlayerMgr, player, new_index);
		if (new_index == 0xFF) {
			g_player_service->player_leave(player);
			if (const auto* net_player_data = player->get_net_data(); net_player_data)
			{
				if (g->notifications.player_leave.log)
					LOG(INFO) << "Player left '" << net_player_data->m_name
					<< "' freeing slot #" << (int)player->m_player_id
					<< " with Rockstar ID: " << net_player_data->m_gamer_handle_2.m_rockstar_id;

				if (g->notifications.player_leave.notify)
					g_notification_service->push("Player Left", fmt::format("{} freeing slot #{} with Rockstar ID: {}", net_player_data->m_name, player->m_player_id, net_player_data->m_gamer_handle_2.m_rockstar_id));
			}
			return returnResult;
		}

		g_player_service->player_join(player);
		if (const auto* net_player_data = player->get_net_data(); net_player_data)
		{
			if (g->notifications.player_join.above_map)
				notify::player_joined(player);

			if (g->notifications.player_join.log)
				LOG(INFO) << "Player joined '" << net_player_data->m_name
				<< "' allocating slot #" << (int)player->m_player_id
				<< " with Rockstar ID: " << net_player_data->m_gamer_handle_2.m_rockstar_id;

			if (g->notifications.player_join.notify)
				g_notification_service->push("Player Joined", fmt::format("{} taking slot #{} with Rockstar ID: {}", net_player_data->m_name, player->m_player_id, net_player_data->m_gamer_handle_2.m_rockstar_id));

			// TODO: Add togle for this
			g_player_database_service->add_player_to_db(net_player_data->m_gamer_handle_2.m_rockstar_id, net_player_data->m_name, "Random");
		}
		return returnResult;
	}
}