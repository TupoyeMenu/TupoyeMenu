/**
 * @file custom_text_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "gta/joaat.hpp"
#include <list>

namespace big
{
	using custom_label_callback = std::function<const char*(const char*)>;
	class custom_text_service final
	{
		std::map<rage::joaat_t, custom_label_callback> m_callbacks;
		std::map<rage::joaat_t, std::unique_ptr<char[]>> m_label_overwrites;

	public:
		custom_text_service();
		~custom_text_service();

		custom_text_service(const custom_text_service&)                = delete;
		custom_text_service(custom_text_service&&) noexcept            = delete;
		custom_text_service& operator=(const custom_text_service&)     = delete;
		custom_text_service& operator=(custom_text_service&&) noexcept = delete;

		bool add_callback_for_label(rage::joaat_t hash, custom_label_callback&& cb);
		bool add_callback_for_labels(std::list<rage::joaat_t> hashes, custom_label_callback&& cb);
		bool add_label_overwrite(rage::joaat_t hash, std::string_view overwrite);

		/**
		 * @brief Get the custom text for a label.
		 * @return nullptr if no custom text exists
		 */
		[[nodiscard]] const char* get_text(const char* label) const;
	};

	inline custom_text_service* g_custom_text_service;
}