/**
 * @file view_debug_script_events.cpp
 * @brief In game gui script event sender.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gui/components/components.hpp"
#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::debug_script_events()
	{
		static int64_t* args;
		static int event_arg_count = 3;
		static int previous_arg_count;
		static int event_player_bits;
		static bool event_everyone = false;

		ImGui::TextUnformatted("Script Argument Count:");
		ImGui::InputInt("###script_event_arg_count", &event_arg_count);
		if (event_arg_count > 32)
			event_arg_count = 32;
		else if (event_arg_count < 3)
			event_arg_count = 3;

		if (event_arg_count != previous_arg_count)
		{
			int64_t* temp_args = new int64_t[event_arg_count]{0};
			memcpy(temp_args, args, sizeof(int64_t) * std::min(event_arg_count, previous_arg_count));

			delete[] args;
			args = temp_args;

			previous_arg_count = event_arg_count;
		}

		ImGui::Separator();

		for (int i = 0; i < event_arg_count; i++)
		{
			ImGui::PushID(i);
			ImGui::Text("Arg[%d]", i);
			ImGui::SameLine();

			ImGui::InputScalar("###input_dynamic_arg", ImGuiDataType_S64, &args[i]);

			ImGui::PopID();
		}

		ImGui::Separator();

		ImGui::Checkbox("Send to everyone", &event_everyone);
		if (!event_everyone)
		{
			ImGui::TextUnformatted("Player ID:");
			ImGui::InputInt("###player_bits", &event_player_bits);
		}

		components::button("Send Event", [] {
			args[1] = self::id; // prevent detection from AC
			g_pointers->m_gta.m_trigger_script_event(1, args, event_arg_count, event_everyone ? -1 : 1 << event_player_bits);
		});
	}
}