/**
 * @file notification_service.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "widgets/imgui_hotkey.hpp"

#include "notification_service.hpp"

namespace big
{
	bool notification_service::initialise()
	{
		push("Welcome",
		    std::format("Loaded TupoyeMenu-lite. Press {} to open", ImGui::key_names[g.settings.hotkeys.menu_toggle]));

		return true;
	}

	void notification_service::push(const std::string& title, const std::string& message)
	{
		push({title, message, NotificationType::INFO});
	}

	void notification_service::push_warning(const std::string& title, const std::string& message)
	{
		push({title, message, NotificationType::WARNING, 7s});
	}

	void notification_service::push_error(const std::string& title, const std::string& message)
	{
		push({title, message, NotificationType::DANGER, 7s});
	}

	void notification_service::push_success(const std::string& title, const std::string& message)
	{
		push({title, message, NotificationType::SUCCESS, 7s});
	}

	std::vector<notification> notification_service::get()
	{
		// remove old notifications
		for (auto it = m_notifications.begin(); it != m_notifications.end();)
		{
			if (it->second.should_be_destroyed())
			{
				it = m_notifications.erase(it++);

				continue;
			}
			++it;
		}

		// grab only the notifications, no need for the keys
		auto sorted = m_notifications | std::views::values | std::ranges::to<std::vector<notification>>();
		std::sort(sorted.begin(), sorted.end(), [](auto const& a, auto const& b) {
			// inverse sorting, highest remaining time goes to top
			return a.destroy_time() > b.destroy_time();
		});
		return sorted;
	}

	void notification_service::push(notification n)
	{
		const auto [pair, inserted] = m_notifications.insert({n.identifier(), n});
		if (!inserted)
		{
			pair->second.reset();
		}
	}

}
