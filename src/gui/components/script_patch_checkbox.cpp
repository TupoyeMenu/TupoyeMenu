/**
 * @file script_patch_checkbox.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "components.hpp"
#include "fiber_pool.hpp"
#include "services/script_patcher/script_patcher_service.hpp"

namespace big
{
	bool components::script_patch_checkbox(const std::string_view text, bool* option, const std::string_view tooltip)
	{
		bool ret = ImGui::Checkbox(text.data(), option);
		if (ret)
		{
			g_fiber_pool->queue_job([] {
				g_script_patcher_service->update();
			});
		}

		if (!tooltip.empty())
		{
			ImGui::SameLine(); components::help_marker(tooltip);
		}

		return ret;
	}
}