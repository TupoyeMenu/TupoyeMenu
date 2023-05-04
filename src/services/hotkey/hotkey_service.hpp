/**
 * @file hotkey_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "common.hpp"
#include "core/enums.hpp"
#include "hotkey.hpp"

namespace big
{
	using hotkey_map = std::multimap<key_t, hotkey>;

	class hotkey_service final
	{
	public:
		hotkey_service();
		virtual ~hotkey_service();
		hotkey_service(const hotkey_service&)                = delete;
		hotkey_service(hotkey_service&&) noexcept            = delete;
		hotkey_service& operator=(const hotkey_service&)     = delete;
		hotkey_service& operator=(hotkey_service&&) noexcept = delete;


		void register_hotkey(const std::string_view name, const key_t initial_key, const rage::joaat_t command_hash, const eKeyState state = eKeyState::RELEASE, std::optional<std::chrono::high_resolution_clock::duration> cooldown = std::nullopt);
		bool update_hotkey(const std::string_view name, const key_t new_key);
		void wndproc(eKeyState state, key_t key);

	private:
		// yes curse me
		std::array<hotkey_map, 2> m_hotkeys;
	};

	inline hotkey_service* g_hotkey_service{};
}
