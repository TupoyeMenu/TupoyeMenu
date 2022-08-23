#pragma once

#include "natives.hpp"
#include "core/data/model_attachment.hpp"


namespace big
{
	struct player_l {
		std::string player_name;
		uint64_t rid;
		std::string relation;
		int tmp_player_id;
	};
	using player_list = std::vector<player_l>;

	using player_database_callback = std::function<const char* (const char*)>;
	class player_database_service
	{
	public:

		player_database_service();
		~player_database_service();

		player_list& player_list_()
		{
			return m_player_list;
		}

		static void save_players();
		static void load_players();

		static void add_player_to_db(uint64_t rid, std::string name, std::string relationship);

		
	private:
		player_list m_player_list;

		static constexpr auto name_key = "name";
		static constexpr auto rid_key = "rid";
		static constexpr auto ipv4_key = "ipv4";

		static constexpr auto relation_key = "relation";
		static constexpr auto frame_flags_key = "frame_flags";

		static constexpr auto players_key = "players";

		static void load_players_from_json(nlohmann::json player_json);

		static nlohmann::json get_player_json(uint64_t rid, Player player, std::string relation);
		static nlohmann::json get_player_json(uint64_t rid, std::string name, std::string relation);

		static nlohmann::json get_player_json_full(uint64_t rid, Player player, std::string relation);
		static nlohmann::json get_player_json_small(uint64_t rid, std::string name, std::string relation);

		static big::folder check_players_folder();
	};

	inline player_database_service* g_player_database_service;
}
