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
namespace big
{
	enum class NotificationType
	{
		INFO,
		SUCCESS,
		WARNING,
		DANGER,
	};

	struct notification
	{
		NotificationType type;
		const std::string title;
		const std::string message;
		const std::chrono::time_point<std::chrono::system_clock> created_on;
		const float destroy_in;
		float alpha;
	};

	class notification_service final
	{
		std::unordered_map<std::size_t, notification> notifications;

	public:
		notification_service();
		virtual ~notification_service();

		void push(notification);
		void push(const std::string&, const std::string&);
		void push_warning(const std::string&, const std::string&);
		void push_error(const std::string&, const std::string&);
		void push_success(const std::string&, const std::string&);
		std::vector<notification> get();

		std::map<NotificationType, ImVec4> notification_colors = {
		    {NotificationType::INFO, ImVec4(0.80f, 0.80f, 0.83f, 1.00f)},
		    {NotificationType::SUCCESS, ImVec4(0.29f, 0.69f, 0.34f, 1.00f)},
		    {NotificationType::WARNING, ImVec4(0.69f, 0.49f, 0.29f, 1.00f)},
		    {NotificationType::DANGER, ImVec4(0.69f, 0.29f, 0.29f, 1.00f)},
		};
	};

	inline notification_service* g_notification_service{};
}