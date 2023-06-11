/**
 * @file persistent_player.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "core/data/infractions.hpp"
#include "json_util.hpp"

#include <unordered_set>

namespace nlohmann
{
	template<typename T>
	struct adl_serializer<std::optional<T>>
	{
		static void to_json(json& j, const std::optional<big::CommandAccessLevel>& opt)
		{
			if (opt == std::nullopt)
			{
				j = nullptr;
			}
			else
			{
				j = *opt;
			}
		}

		static void from_json(const json& j, std::optional<big::CommandAccessLevel>& opt)
		{
			if (j.is_null())
			{
				opt = std::nullopt;
			}
			else
			{
				opt = j.get<big::CommandAccessLevel>();
			}
		}
	};
}

namespace big
{
	enum class PlayerOnlineStatus
	{
		UNKNOWN,
		OFFLINE,
		ONLINE
	};

	struct persistent_player
	{
		std::string name;
		std::uint64_t rockstar_id = 0;
		bool block_join           = false;
		int block_join_reason     = 1;
		bool force_allow_join     = false;
		bool is_modder            = false;
		bool notify_online		  = false;
		std::unordered_set<int> infractions;
		std::optional<CommandAccessLevel> command_access_level = std::nullopt;
		PlayerOnlineStatus online_state                        = PlayerOnlineStatus::UNKNOWN;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(persistent_player, name, rockstar_id, block_join, block_join_reason, force_allow_join, is_modder, notify_online, infractions, command_access_level)
	};

};
