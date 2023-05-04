/**
 * @file pickup_service.hpp
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
	class pickup_service final
	{
	public:
		pickup_service();
		~pickup_service();

		pickup_service(const pickup_service&)                = delete;
		pickup_service(pickup_service&&) noexcept            = delete;
		pickup_service& operator=(const pickup_service&)     = delete;
		pickup_service& operator=(pickup_service&&) noexcept = delete;

		void give_player_ammo(const Player player) const;
		void give_player_armour(const Player player) const;
		void give_player_health(const Player player) const;
		void give_player_weapons(const Player player) const;

		void give_ammo(const int targets) const;
		void give_armour(const int targets) const;
		void give_health(const int targets) const;
		void give_weapons(const int targets) const;
	};

	inline pickup_service* g_pickup_service{};
}