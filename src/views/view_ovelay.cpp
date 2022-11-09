#include "views/view.hpp"
#include "natives.hpp"
#include "fiber_pool.hpp"
#include "services/players/player_service.hpp"

namespace big
{
    void view::overlay()
    {
        if (!g->window.overlay) return;
        static int corner = 0;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("overlay", &g->window.overlay, window_flags))
        {
            const auto player_count = g_player_service->players().size() + 1;
            ImGui::Text("Players: %i", player_count);
            //ImGui::Text("Speed: (%.1f)", ENTITY::GET_ENTITY_SPEED(self::ped));
            ImGui::Text("Coords: (X:%.1f, Y:%.1f, Z:%.1f)", self::pos.x, self::pos.y, self::pos.z);
            ImGui::Separator();
            ImGui::Text("FPS: %.1f  Frame Time: %.1f", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);


            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
                if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
                if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
                if (g->window.overlay && ImGui::MenuItem("Close")) g->window.overlay = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
}
