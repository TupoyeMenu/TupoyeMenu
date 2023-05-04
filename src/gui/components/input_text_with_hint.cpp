/**
 * @file input_text_with_hint.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "fiber_pool.hpp"
#include "gui/components/components.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include "natives.hpp"

namespace big
{
	void components::input_text_with_hint(const std::string_view label, const std::string_view hint, char* buf, size_t buf_size, ImGuiInputTextFlags flag, std::function<void()> cb)
	{
		if (ImGui::InputTextWithHint(label.data(), hint.data(), buf, buf_size, flag))
			if (cb)
				g_fiber_pool->queue_job(std::move(cb));

		if (ImGui::IsItemActive())
			g_fiber_pool->queue_job([] {
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			});
	}

	void components::input_text_with_hint(const std::string_view label, const std::string_view hint, std::string* buf, ImGuiInputTextFlags flag, std::function<void()> cb)
	{
		if (ImGui::InputTextWithHint(label.data(), hint.data(), buf, flag))
			if (cb)
				g_fiber_pool->queue_job(std::move(cb));

		if (ImGui::IsItemActive())
			g_fiber_pool->queue_job([] {
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			});
	}
}