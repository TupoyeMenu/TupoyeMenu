#include "views/view.hpp"
#include "renderer.hpp"

namespace big
{
	void view::gui_settings()
	{
		components::sub_title("UI Scale");
		ImGui::SliderFloat("##gui-scale", &g.window.gui_scale, 1.f, 1.5f, "%.2f");
		ImGui::SameLine();
		if (ImGui::Button("Apply##gui-scale"))
			g_renderer->rescale(g.window.gui_scale);
		ImGui::SameLine(); components::help_marker("Changing the UI scale may break rendering and require you to unload and inject YimMenu again.");

		components::sub_title("Colors");
		static ImVec4 col_gui = ImGui::ColorConvertU32ToFloat4(g.window.color);
		if (ImGui::ColorEdit4("Gui Color##gui_picker", (float*)&col_gui, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.window.color = ImGui::ColorConvertFloat4ToU32(col_gui);
		}

		components::sub_title("Ingame Overlay");
		ImGui::Checkbox("Show Overlay", &g.window.ingame_overlay.opened);
		ImGui::SameLine();
		ImGui::Checkbox("Show when Menu opened", &g.window.ingame_overlay.show_with_menu_opened);

		ImGui::BeginGroup();

		ImGui::Text("Position");
		if (ImGui::Button("Custom")) g.window.ingame_overlay.corner = -1;
        if (ImGui::Button("Top-left")) g.window.ingame_overlay.corner = 0;
		ImGui::SameLine();
        if (ImGui::Button("Top-right")) g.window.ingame_overlay.corner = 1;
        if (ImGui::Button("Bottom-left")) g.window.ingame_overlay.corner = 2;
		ImGui::SameLine();
        if (ImGui::Button("Bottom-right")) g.window.ingame_overlay.corner = 3;

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
		ImGui::Checkbox("Show Game Version", &g.window.ingame_overlay.show_game_versions);

		ImGui::EndGroup();
	}

}