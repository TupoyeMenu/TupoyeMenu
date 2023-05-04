/**
 * @file hotkey.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "backend/command.hpp"
#include "common.hpp"
#include "rage/joaat.hpp"

namespace big
{
	using key_t = unsigned int;

	class hotkey final
	{
	public:
		hotkey(rage::joaat_t name_hash, key_t key, rage::joaat_t command_hash, std::optional<std::chrono::high_resolution_clock::duration> cooldown = std::nullopt);
		virtual ~hotkey() = default;

		bool can_exec() const;
		void exec();

		rage::joaat_t name_hash() const;
		void set_key(key_t new_key);

	private:
		rage::joaat_t m_name_hash;
		key_t m_key;

		rage::joaat_t m_command_hash;
		std::optional<std::chrono::high_resolution_clock::duration> m_cooldown;
		std::chrono::high_resolution_clock::time_point m_wakeup;
	};
}