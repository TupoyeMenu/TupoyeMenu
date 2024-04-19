/**
 * @file view_cmd_executor.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "backend/context/console_command_context.hpp"
#include "gui.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "services/hotkey/hotkey_service.hpp"
#include "views/view.hpp"

#include "imgui_internal.h"

namespace big
{
	ImVec4 get_log_color(const eLogLevel level)
	{
		switch (level)
		{
		case VERBOSE: return ImVec4(0.0f, 0.44f, 0.85f, 1.0f);
		case INFO: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		case WARNING: return ImVec4(1.0f, 0.7f, 0.0f, 1.0f);
		case FATAL: return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
		}
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void view::cmd_executor()
	{
		if (!g.cmd_executor.enabled)
			return;

		ImGui::SetNextWindowSize(ImVec2(496, 746), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Console", &g.cmd_executor.enabled))
		{
			g_fiber_pool->queue_job([] { // Disable control here because we are not always focused.
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
				PAD::DISABLE_ALL_CONTROL_ACTIONS(2);
			});

			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild("##scrolling_region_log", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

			for (auto msg : g_log.get_log_messages())
			{
				ImGui::PushStyleColor(ImGuiCol_Text, get_log_color(msg->Level()));
				ImGui::TextUnformatted(msg->Message().c_str());
				ImGui::PopStyleColor();
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::PopStyleVar();
			ImGui::EndChild();

			ImGui::Separator();

			static std::string command_buffer = "";
			bool reset_focus = false;
			bool is_input_text_enter_pressed = false;
			if(ImGui::InputTextWithHint("##command_input", "Type your command", &command_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				command::process(command_buffer, std::make_shared<console_command_context>(), false);
				command_buffer = "";
				reset_focus = true;
				is_input_text_enter_pressed = true;
			}

			if(reset_focus || ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere(-1);

			const bool is_input_text_active = ImGui::IsItemActive();
			if (ImGui::IsItemActivated())
				ImGui::OpenPopup("##autocomplete");

			// FIXME: This thing breaks everything need to disable keyboard and make manual navigation.
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
			if (ImGui::BeginPopup("##autocomplete", ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_ChildWindow))
			{
				auto possible_commands = command::get_suggestions(command_buffer);

				for (auto cmd : possible_commands)
				{
					if (components::selectable(cmd->get_name(), false))
					{
						reset_focus = true;
						command_buffer = cmd->get_name();
					}
				}

				if (is_input_text_enter_pressed || (!is_input_text_active && !ImGui::IsWindowFocused()))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	bool_command
	    g_cmd_executor("cmdexecutor", "Toggle Command Executor", "Toggles the command executor window", g.cmd_executor.enabled, false);
}