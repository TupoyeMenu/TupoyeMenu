#include "views/view.hpp"
#include "natives.hpp"
#include "fiber_pool.hpp"
#include "pointers.hpp"
#include "services/players/player_service.hpp"


namespace big
{
    void view::overlay()
    {
        if (!g.window.overlay) return;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (g.window.overlay_corner != -1)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (g.window.overlay_corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (g.window.overlay_corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (g.window.overlay_corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (g.window.overlay_corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("overlay", &g.window.overlay, window_flags))
        {
            const auto player_count = g_player_service->players().size() + 1;
            ImGui::Text("Players: %i", player_count);
            ImGui::Text("Coords: (X:%.1f, Y:%.1f, Z:%.1f)", self::pos.x, self::pos.y, self::pos.z);
            ImGui::Separator();
            ImGui::Text("FPS: %.1f  Frame Time: %.1f", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
            if (const auto replay = *g_pointers->m_replay_interface; replay != nullptr && g.window.overlay_pools)
		    {
			    const auto veh_interface = replay->m_vehicle_interface;
			    const auto ped_interface = replay->m_ped_interface;
			    const auto obj_interface = replay->m_object_interface;
                if(veh_interface != nullptr && ped_interface != nullptr && obj_interface != nullptr)
                {
                    ImGui::Separator();
                    ImGui::Text("Vehicle Pool: %i / %i", veh_interface->m_cur_vehicles , veh_interface->m_max_vehicles);
                    ImGui::Text("Ped Pool: %i / %i", ped_interface->m_cur_peds , ped_interface->m_max_peds);
                    ImGui::Text("Object Pool: %i / %i", obj_interface->m_cur_objects , obj_interface->m_max_objects);
                }
            }


            if (ImGui::BeginPopupContextWindow())
            {
                ImGui::MenuItem("Pool Debug", nullptr, &g.window.overlay_pools);
                if (ImGui::MenuItem("Custom", nullptr, g.window.overlay_corner == -1)) g.window.overlay_corner = -1;
                if (ImGui::MenuItem("Top-left", nullptr, g.window.overlay_corner == 0)) g.window.overlay_corner = 0;
                if (ImGui::MenuItem("Top-right", nullptr, g.window.overlay_corner == 1)) g.window.overlay_corner = 1;
                if (ImGui::MenuItem("Bottom-left", nullptr, g.window.overlay_corner == 2)) g.window.overlay_corner = 2;
                if (ImGui::MenuItem("Bottom-right", nullptr, g.window.overlay_corner == 3)) g.window.overlay_corner = 3;
                if (g.window.overlay && ImGui::MenuItem("Close")) g.window.overlay = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
}
