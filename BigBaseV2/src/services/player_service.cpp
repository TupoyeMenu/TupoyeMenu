#include "gta_util.hpp"
#include "player_service.hpp"
#include "util/notify.hpp"

namespace big
{
	player::player(CNetGamePlayer* net_game_player)
		: m_net_game_player(net_game_player)
	{

	}

	CAutomobile* player::get_current_vehicle()
	{
		if (CPed* ped = this->get_ped(); ped != nullptr)
			if (CAutomobile* vehicle = ped->m_vehicle; vehicle != nullptr)
				return vehicle;
		return nullptr;
	}

	const char* player::get_name()
	{
		return m_net_game_player->get_name();
	}

	rage::netPlayerData* player::get_net_data()
	{
		return m_net_game_player->get_net_data();
	}

	CNetGamePlayer* player::get_net_game_player()
	{
		return m_net_game_player;
	}

	CPed* player::get_ped()
	{
		if (CPlayerInfo* player_info = this->get_player_info(); player_info != nullptr)
			if (CPed* ped = player_info->m_ped; ped != nullptr)
				return ped;
		return nullptr;
	}

	CPlayerInfo* player::get_player_info()
	{
		if (m_net_game_player != nullptr && m_net_game_player->m_player_info != nullptr)
			return m_net_game_player->m_player_info;
		return nullptr;
	}

	uint8_t player::id()
	{
		return m_net_game_player->m_player_id;
	}

	bool player::is_host()
	{
		return m_net_game_player->is_host();
	}

	bool player::is_valid()
	{
		return m_net_game_player == nullptr ? false : m_net_game_player->is_valid();
	}

	std::string player::to_lowercase_identifier()
	{
		std::string lower = this->get_name();
		std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

		return lower;
	}

	player_service::player_service()
	{
		if (CNetworkPlayerMgr* network_player_mgr = gta_util::get_network_player_mgr())
		{
			for (size_t i = 0; i < network_player_mgr->m_player_limit; i++)
			{
				if (CNetGamePlayer* net_game_player = network_player_mgr->m_player_list[i]; net_game_player != nullptr)
				{
					std::unique_ptr<player> plyr = std::make_unique<player>(net_game_player);

					m_players.emplace(
						plyr->to_lowercase_identifier(),
						std::move(plyr)
					);
				}
			}
		}

		g_player_service = this;
	}

	player_service::~player_service()
	{
		g_player_service = nullptr;
	}

	void player_service::do_cleanup()
	{
		m_players.clear();
	}

	void player_service::player_join(CNetGamePlayer* net_game_player)
	{
		if (net_game_player == nullptr) return;
		notify::player_joined(net_game_player);

		std::unique_ptr<player> plyr = std::make_unique<player>(net_game_player);

		m_players.emplace(
			plyr->to_lowercase_identifier(),
			std::move(plyr)
		);
	}

	void player_service::player_leave(CNetGamePlayer* net_game_player)
	{
		if (net_game_player == nullptr) return;

		std::unique_ptr<player> plyr = std::make_unique<player>(net_game_player);

		m_players.erase(plyr->to_lowercase_identifier());
	}
}