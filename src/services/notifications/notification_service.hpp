/**
 * @file notification_service.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "notification.hpp"

namespace big
{

	class notification_service final
	{
		std::unordered_map<std::size_t, notification> m_notifications;

	public:
		notification_service() = default;
		virtual ~notification_service() = default;

		bool initialise();

		void push(const std::string& title, const std::string& message);
		void push_warning(const std::string& title, const std::string& message);
		void push_error(const std::string& title, const std::string& message);
		void push_success(const std::string& title, const std::string& message);

		// cleans up old notifications from the map and returns a sorted list based on the destroy time
		std::vector<notification> get();

	private:
		void push(notification notification);

	};

	inline notification_service g_notification_service{};
}
