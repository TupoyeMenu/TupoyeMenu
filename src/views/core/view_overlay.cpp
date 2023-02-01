#include "views/view.hpp"
#include "pointers.hpp"
#include "gta_util.hpp"
#include "gui.hpp"

namespace big
{
	void view::overlay()
	{
		if (!g.window.ingame_overlay.opened || (g_gui->is_open() && !g.window.ingame_overlay.show_with_menu_opened))
			return;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (g.window.ingame_overlay.corner != -1)
		{
			const float PAD = 10.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (g.window.ingame_overlay.corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (g.window.ingame_overlay.corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (g.window.ingame_overlay.corner & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (g.window.ingame_overlay.corner & 2) ? 1.0f : 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowViewport(viewport->ID);
			window_flags |= ImGuiWindowFlags_NoMove;
		}

		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("overlay", nullptr, window_flags))
		{
			if (g.window.ingame_overlay.show_watermark)
			{
				ImGui::Text("TupoyeMenu");
				ImGui::Separator();
			}

			if (CNetworkPlayerMgr *network_player_mgr = gta_util::get_network_player_mgr(); network_player_mgr != nullptr && g.window.ingame_overlay.show_players)
				ImGui::Text("Players: %i/%i", network_player_mgr->m_player_count, network_player_mgr->m_player_limit);

			if(g.window.ingame_overlay.show_coords)
				ImGui::Text("Coords: (X:%.1f, Y:%.1f, Z:%.1f)", self::pos.x, self::pos.y, self::pos.z);

			if (g.window.ingame_overlay.show_fps)
				ImGui::Text("FPS: %.1f  Frame Time: %.1f", ImGui::GetIO().Framerate / 2, 1000.0f / ImGui::GetIO().Framerate);

			//if (g.window.ingame_overlay.show_time)
			//	ImGui::Text(std::format("Time: {:%d-%m-%Y %H:%M:%OS}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())).c_str());

			if (auto replay_interface = *g_pointers->m_replay_interface; replay_interface != nullptr && g.window.ingame_overlay.show_replay_interface)
			{
				if(replay_interface->m_ped_interface || replay_interface->m_vehicle_interface || replay_interface->m_object_interface)
				ImGui::Separator();

				if(replay_interface->m_ped_interface)
					ImGui::Text("Ped Pool: %i/%i", replay_interface->m_ped_interface->m_cur_peds, replay_interface->m_ped_interface->m_max_peds);

				if(replay_interface->m_vehicle_interface)
					ImGui::Text("Vehicle Pool: %i/%i", replay_interface->m_vehicle_interface->m_cur_vehicles, replay_interface->m_vehicle_interface->m_max_vehicles);
					
				if(replay_interface->m_object_interface)
					ImGui::Text("Object Pool: %i/%i", replay_interface->m_object_interface->m_cur_objects, replay_interface->m_object_interface->m_max_objects);
			}
	
			if (g.window.ingame_overlay.show_game_versions)
			{
				ImGui::Separator();
				ImGui::Text("Game Version: %s", g_pointers->m_game_version);
				ImGui::Text("Online Version: %s", g_pointers->m_online_version);
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, g.window.ingame_overlay.corner == -1)) g.window.ingame_overlay.corner = -1;
				if (ImGui::MenuItem("Top-left", NULL, g.window.ingame_overlay.corner == 0)) g.window.ingame_overlay.corner = 0;
				if (ImGui::MenuItem("Top-right", NULL, g.window.ingame_overlay.corner == 1)) g.window.ingame_overlay.corner = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, g.window.ingame_overlay.corner == 2)) g.window.ingame_overlay.corner = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, g.window.ingame_overlay.corner == 3)) g.window.ingame_overlay.corner = 3;
				if (ImGui::MenuItem("Close")) g.window.ingame_overlay.opened = false;
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}