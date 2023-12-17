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
#include "json_util.hpp"

#include <cstdint>
#include <unordered_set>

namespace big
{
	struct logged_player
	{
		std::string name;
		uint32_t ip                   = 0;
		uint64_t rid                  = 0;
		uint64_t last_seen            = 0;
		bool is_modder                = false;
		std::string infraction_reason = "";

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(logged_player, name, rid, ip, last_seen, is_modder, infraction_reason)
	};
};
