/**
 * @file selectable.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "fiber_pool.hpp"
#include "gui/components/components.hpp"

namespace big
{
	bool components::selectable(const std::string_view text, bool selected)
	{
		return ImGui::Selectable(text.data(), selected);
	}

	bool components::selectable(const std::string_view text, bool selected, ImGuiSelectableFlags flag)
	{
		return ImGui::Selectable(text.data(), selected, flag);
	}

	void components::selectable(const std::string_view text, bool selected, std::function<void()> cb)
	{
		if (components::selectable(text, selected))
			g_fiber_pool->queue_job(std::move(cb));
	}

	void components::selectable(const std::string_view text, bool selected, ImGuiSelectableFlags flag, std::function<void()> cb)
	{
		if (components::selectable(text, selected, flag))
		{
			g_fiber_pool->queue_job(std::move(cb));
		}
	}
}