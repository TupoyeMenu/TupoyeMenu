/**
 * @file hotkey.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hotkey.hpp"

namespace big
{
	hotkey::hotkey(rage::joaat_t name_hash, key_t key, rage::joaat_t command_hash, std::optional<std::chrono::high_resolution_clock::duration> cooldown) :
	    m_name_hash(name_hash),
	    m_key(key),
	    m_command_hash(command_hash),
	    m_cooldown(cooldown),
	    m_wakeup()
	{
	}

	bool hotkey::can_exec() const
	{
		return !m_cooldown.has_value() || std::chrono::high_resolution_clock::now() >= m_wakeup;
	}

	void hotkey::exec()
	{
		if (m_cooldown.has_value())
			m_wakeup = std::chrono::high_resolution_clock::now() + m_cooldown.value();

		command::get(m_command_hash)->call(std::vector<std::uint64_t>());
	}

	rage::joaat_t hotkey::name_hash() const
	{
		return m_name_hash;
	}

	void hotkey::set_key(key_t new_key)
	{
		m_key = new_key;
	}
}