/**
 * @file view_gui_settings.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "imgui.h"
#include "renderer.hpp"
#include "views/view.hpp"

namespace big
{
	void view::gui_settings()
	{
		ImGui::Text("UI Scale");
		if (ImGui::SliderFloat("##gui-scale", &g.window.gui_scale, 1.f, 1.5f, "%.2f"))
			g_renderer->rescale(g.window.gui_scale);
		ImGui::SameLine();
		components::help_marker("Changing the UI scale in game will crash you, don't touch this!.\nOk maybe crashes are linux only.");

		ImGui::SeparatorText("Colors");
		static ImVec4 col_gui = ImGui::ColorConvertU32ToFloat4(g.window.color);
		if (ImGui::ColorEdit4("Gui Color", (float*)&col_gui, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.window.color = ImGui::ColorConvertFloat4ToU32(col_gui);
		}

		ImGui::SeparatorText("Overlay");
		ImGui::Checkbox("Show Overlay", &g.window.ingame_overlay.opened);
		ImGui::SameLine();
		ImGui::Checkbox("Show when Menu opened", &g.window.ingame_overlay.show_with_menu_opened);

		ImGui::BeginGroup();

		ImGui::Text("Position");
		if (ImGui::Button("Custom"))
			g.window.ingame_overlay.corner = -1;
		if (ImGui::Button("Top-left"))
			g.window.ingame_overlay.corner = 0;
		ImGui::SameLine();
		if (ImGui::Button("Top-right"))
			g.window.ingame_overlay.corner = 1;
		if (ImGui::Button("Bottom-left"))
			g.window.ingame_overlay.corner = 2;
		ImGui::SameLine();
		if (ImGui::Button("Bottom-right"))
			g.window.ingame_overlay.corner = 3;

		ImGui::EndGroup();

		ImGui::BeginGroup();

		ImGui::Checkbox("Show Watermark", &g.window.ingame_overlay.show_watermark);
		ImGui::Checkbox("Show Players", &g.window.ingame_overlay.show_players);
		ImGui::Checkbox("Show Coords", &g.window.ingame_overlay.show_coords);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Show FPS", &g.window.ingame_overlay.show_fps);
		ImGui::Checkbox("Show Replay Interface", &g.window.ingame_overlay.show_replay_interface);
		ImGui::Checkbox("Show Game Version", &g.window.ingame_overlay.show_game_versions);

		ImGui::EndGroup();
	}

}