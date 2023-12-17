/**
 * @file all_player_database_service.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "all_player_database_service.hpp"

#include "backend/bool_command.hpp"
#include "file_manager.hpp"
#include "gta/enums.hpp"
#include "hooking.hpp"

namespace big
{
	all_player_database_service::all_player_database_service() :
	    m_file_path(g_file_manager.get_project_file("./players_all.json").get_path())
	{
		load();

		g_all_player_database_service = this;
	}

	all_player_database_service::~all_player_database_service()
	{
		g_all_player_database_service = nullptr;
	}

	void all_player_database_service::save()
	{
		nlohmann::json json;

		for (auto& [rid, player] : m_players)
		{
			json[std::to_string(rid)] = player;
		}

		std::ofstream file_stream(m_file_path);
		file_stream << json.dump();
	}

	void all_player_database_service::load()
	{
		if (std::filesystem::exists(m_file_path))
		{
			try
			{
				std::ifstream file_stream(m_file_path);

				nlohmann::json json;
				file_stream >> json;
				file_stream.close();

				for (auto& [key, value] : json.items())
				{
					auto player                = value.get<std::shared_ptr<logged_player>>();
					m_players[std::stoll(key)] = player;

					std::string lower = player->name;
					std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
				}
			}
			catch (std::exception& e)
			{
				LOG(WARNING) << "Failed to load all player database file. " << e.what();
			}
		}
	}

	std::unordered_map<uint64_t, std::shared_ptr<logged_player>>& all_player_database_service::get_players()
	{
		return m_players;
	}

	std::shared_ptr<logged_player> all_player_database_service::add_player(const std::uint64_t rid, const std::string_view name, const std::uint32_t ip, const std::uint32_t port)
	{
		std::string lower = name.data();
		std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

		std::time_t cur_time = std::time(0);

		auto player = std::make_shared<logged_player>(name.data(), ip, rid, cur_time);

		m_players[rid] = player;

		return player;
	}

	std::shared_ptr<logged_player> all_player_database_service::get_player_by_rockstar_id(uint64_t rockstar_id)
	{
		if (m_players.contains(rockstar_id))
			return m_players[rockstar_id];
		return nullptr;
	}

	std::shared_ptr<logged_player> all_player_database_service::get_or_create_player(player_ptr player)
	{
		if (m_players.contains(player->get_net_data()->m_gamer_handle.m_rockstar_id))
			return m_players[player->get_net_data()->m_gamer_handle.m_rockstar_id];
		else
		{
			auto ip   = g_player_service->get_selected()->get_ip_address();
			auto port = g_player_service->get_selected()->get_port();

			auto player_ptr = add_player(player->get_net_data()->m_gamer_handle.m_rockstar_id, player->get_name(), ip->m_packed, port);

			save();
			return player_ptr;
		}
	}

	void all_player_database_service::update_rockstar_id(uint64_t old, uint64_t _new)
	{
		auto player  = m_players.extract(old);
		player.key() = _new;

		m_players.insert(std::move(player));
	}

	void all_player_database_service::remove_rockstar_id(uint64_t rockstar_id)
	{
		if (auto it = m_players.find(rockstar_id); it != m_players.end())
		{
			std::string lower = it->second->name;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

			m_players.erase(it);
		}
	}
}
