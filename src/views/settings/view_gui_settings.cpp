#include "gui.hpp"
#include "renderer/renderer.hpp"
#include "views/view.hpp"

namespace big
{
	void view::gui_settings()
	{
		ImGui::TextUnformatted("UI Scale");
		if (ImGui::SliderFloat("##gui-scale", &g.window.gui_scale, 0.75f, 1.5f, "%.2f"))
			g_renderer.rescale(g.window.gui_scale);
		ImGui::SameLine();
		components::help_marker("Changing the UI scale in game will crash wine users.");

		ImGui::SeparatorText("Colors");
		static ImVec4 col_gui = ImGui::ColorConvertU32ToFloat4(g.window.background_color);
		if (ImGui::ColorEdit4("Gui Color", (float*)&col_gui, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
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

		ImGui::SeparatorText("Overlay");

		ImGui::Checkbox("Show Overlay", &g.window.ingame_overlay.opened);
		ImGui::SameLine();
		ImGui::Checkbox("Show When Menu Opened", &g.window.ingame_overlay.show_with_menu_opened);

		ImGui::BeginGroup();

		ImGui::TextUnformatted("Position");
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
		ImGui::Checkbox("Show Position", &g.window.ingame_overlay.show_position);
		ImGui::Checkbox("Show Indicators", &g.window.ingame_overlay.show_indicators);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Show FPS", &g.window.ingame_overlay.show_fps);
		ImGui::Checkbox("Show Replay Interface", &g.window.ingame_overlay.show_replay_interface);
		ImGui::Checkbox("Show Game Version", &g.window.ingame_overlay.show_game_versions);

		ImGui::EndGroup();

		ImGui::Checkbox("Format Money", &g.window.gui.format_money);

		if (g.window.ingame_overlay.show_indicators)
		{
			if (ImGui::TreeNode("Overlay Indicators"))
			{
				ImGui::BeginGroup();
				ImGui::Checkbox("Show Player Godmode", &g.window.ingame_overlay.ingame_overlay_indicators.show_player_godmode);
				ImGui::Checkbox("Show Off Radar", &g.window.ingame_overlay.ingame_overlay_indicators.show_off_radar);
				ImGui::Checkbox("Show Vehicle Godmode", &g.window.ingame_overlay.ingame_overlay_indicators.show_vehicle_godmode);
				ImGui::Checkbox("Show Never Wanted", &g.window.ingame_overlay.ingame_overlay_indicators.show_never_wanted);
				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::Checkbox("Show Infinite Ammo", &g.window.ingame_overlay.ingame_overlay_indicators.show_infinite_ammo);
				ImGui::Checkbox("Show Always Full Ammo", &g.window.ingame_overlay.ingame_overlay_indicators.show_always_full_ammo);
				ImGui::Checkbox("Show Infinite Magazine", &g.window.ingame_overlay.ingame_overlay_indicators.show_infinite_mag);
				ImGui::Checkbox("Show Invisibility", &g.window.ingame_overlay.ingame_overlay_indicators.show_invisibility);
				ImGui::EndGroup();

				ImGui::TreePop();
			}
		}
	}

}
