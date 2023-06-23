#include "gui.hpp"
#include "renderer.hpp"
#include "views/view.hpp"

namespace big
{
	void view::gui_settings()
	{
		components::sub_title("SETTINGS_UI_SCALE"_T);
		if (ImGui::SliderFloat("##gui-scale", &g.window.gui_scale, 0.75f, 1.5f, "%.2f"))
			g_renderer->rescale(g.window.gui_scale);
		ImGui::SameLine();
		components::help_marker("SETTINGS_UI_SCALE_DESCRIPTION"_T);

		components::sub_title("SETTINGS_UI_COLOR"_T);
		static ImVec4 col_gui = ImGui::ColorConvertU32ToFloat4(g.window.background_color);
		if (ImGui::ColorEdit4("SETTINGS_UI_COLOR_PICKER"_T.data(), (float*)&col_gui, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.window.background_color = ImGui::ColorConvertFloat4ToU32(col_gui);
		}

		static ImVec4 col_text = ImGui::ColorConvertU32ToFloat4(g.window.text_color);
		if (ImGui::ColorEdit4("Text Color", (float*)&col_text, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.window.text_color = ImGui::ColorConvertFloat4ToU32(col_text);
		}

		static ImVec4 col_button = ImGui::ColorConvertU32ToFloat4(g.window.button_color);
		if (ImGui::ColorEdit4("Button Color", (float*)&col_button, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.window.button_color = ImGui::ColorConvertFloat4ToU32(col_button);
		}

		static ImVec4 col_frame = ImGui::ColorConvertU32ToFloat4(g.window.frame_color);
		if (ImGui::ColorEdit4("Frame Color", (float*)&col_frame, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.window.frame_color = ImGui::ColorConvertFloat4ToU32(col_frame);
		}

		components::sub_title("In-Game Overlay");
		ImGui::Checkbox("Show Overlay", &g.window.ingame_overlay.opened);
		ImGui::SameLine();
		ImGui::Checkbox("Show When Menu Opened", &g.window.ingame_overlay.show_with_menu_opened);

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

		ImGui::Checkbox("Show Coords", &g.window.ingame_overlay.show_coords);
		ImGui::Checkbox("Show Replay Interface", &g.window.ingame_overlay.show_replay_interface);
		ImGui::Checkbox("Show Position", &g.window.ingame_overlay.show_position);
		ImGui::Checkbox("Show Game Version", &g.window.ingame_overlay.show_game_versions);

		ImGui::EndGroup();
	}

}