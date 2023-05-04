/**
 * @file friends_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace big
{
	class friends_service final
	{
	public:
		friends_service();
		~friends_service();

		friends_service(const friends_service&)                = delete;
		friends_service(friends_service&&) noexcept            = delete;
		friends_service& operator=(const friends_service&)     = delete;
		friends_service& operator=(friends_service&&) noexcept = delete;

		[[nodiscard]] static bool is_friend(CNetGamePlayer* net_player);
	};

	inline friends_service* g_friends_service{};
}