/**
 * @file player_database_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once
#include "logged_player.hpp"
#include "services/players/player.hpp"

// To fix json problems
#include "services/player_database/player_database_service.hpp"

namespace big
{
	class all_player_database_service
	{
		std::unordered_map<uint64_t, std::shared_ptr<logged_player>> m_players;

	public:
		std::filesystem::path m_file_path;
		all_player_database_service();
		~all_player_database_service();

		void save();
		void load();


		std::shared_ptr<logged_player> add_player(const std::uint64_t rid, const std::string_view name, const std::uint32_t ip, const std::uint32_t port);
		std::unordered_map<uint64_t, std::shared_ptr<logged_player>>& get_players();
		std::map<std::string, std::shared_ptr<logged_player>>& get_sorted_players();
		std::shared_ptr<logged_player> get_player_by_rockstar_id(uint64_t rockstar_id);
		std::shared_ptr<logged_player> get_or_create_player(player_ptr player);
		void update_rockstar_id(uint64_t old, uint64_t _new);
		void remove_rockstar_id(uint64_t rockstar_id);
	};

	inline all_player_database_service* g_all_player_database_service;
}