#include "gta/pools.hpp"
#include "gta_util.hpp"
#include "gui.hpp"
#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::overlay()
	{
		if (!g.window.ingame_overlay.opened || (g_gui->is_open() && !g.window.ingame_overlay.show_with_menu_opened))
			return;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (g.window.ingame_overlay.corner != -1)
		{
			const float PAD               = 10.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos               = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size              = viewport->WorkSize;
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
		g_gui->push_theme_colors();
		if (ImGui::Begin("overlay", nullptr, window_flags))
		{
			if (g.window.ingame_overlay.show_watermark)
			{
				ImGui::SeparatorText("TupoyeMenu");
			}

			if (CNetworkPlayerMgr* network_player_mgr = gta_util::get_network_player_mgr();
			    network_player_mgr != nullptr && g.window.ingame_overlay.show_players)
				ImGui::Text("Players: %i/%i", network_player_mgr->m_player_count, network_player_mgr->m_player_limit);

			if (g.window.ingame_overlay.show_position)
				ImGui::Text("Coords: (X:%.1f, Y:%.1f, Z:%.1f)", self::pos.x, self::pos.y, self::pos.z);

			if (g.window.ingame_overlay.show_fps)
				ImGui::Text("FPS: %.1f  Frame Time: %.1f", ImGui::GetIO().Framerate / 2, 1000.0f / ImGui::GetIO().Framerate);

			if (CNetworkPlayerMgr* network_player_mgr = gta_util::get_network_player_mgr(); g.window.ingame_overlay.show_players)
				ImGui::Text(std::format("Players: {}/{}", network_player_mgr->m_player_count, network_player_mgr->m_player_limit)
				                .c_str());

			if (g.window.ingame_overlay.show_indicators)
			{
				ImGui::Separator();

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_player_godmode)
					components::overlay_indicator("Player Godmode", g.self.god_mode);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_off_radar)
					components::overlay_indicator("Off Radar", g.self.off_radar);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_vehicle_godmode)
					components::overlay_indicator("Vehicle Godmode", g.vehicle.god_mode);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_never_wanted)
					components::overlay_indicator("Never Wanted", g.self.never_wanted);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_infinite_ammo)
					components::overlay_indicator("Infinite Ammo", g.weapons.infinite_ammo);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_always_full_ammo)
					components::overlay_indicator("Always Full Ammo", g.weapons.always_full_ammo);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_infinite_mag)
					components::overlay_indicator("Infinite Magazine", g.weapons.infinite_mag);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_aimbot)
					components::overlay_indicator("Aimbot", g.weapons.aimbot.enable);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_triggerbot)
					components::overlay_indicator("Triggerbot", g.weapons.triggerbot);

				if (g.window.ingame_overlay.ingame_overlay_indicators.show_invisibility)
					components::overlay_indicator("Invisibility", g.self.invisibility);
			}

			if (g.window.ingame_overlay.show_position && g_local_player)
			{
				ImGui::Separator();

				auto& pos = *g_local_player->get_position();

				ImGui::Text("Pos: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
			}

			if (g.window.ingame_overlay.show_replay_interface)
			{
				if (*g_pointers->m_gta.m_ped_pool || (*g_pointers->m_gta.m_vehicle_pool && **g_pointers->m_gta.m_vehicle_pool)
				    || *g_pointers->m_gta.m_prop_pool)
					ImGui::Separator();

				if (*g_pointers->m_gta.m_ped_pool)
					ImGui::Text("Ped Pool: %i/%i",
					    (*g_pointers->m_gta.m_ped_pool)->get_item_count(),
					    (*g_pointers->m_gta.m_ped_pool)->m_size);

				if (*g_pointers->m_gta.m_vehicle_pool && **g_pointers->m_gta.m_vehicle_pool)
					ImGui::Text("Vehicle Pool: %i/%i",
					    (**g_pointers->m_gta.m_vehicle_pool)->m_item_count,
					    (**g_pointers->m_gta.m_vehicle_pool)->m_size);

				if (*g_pointers->m_gta.m_prop_pool)
					ImGui::Text("Object Pool: %i/%i",
					    (*g_pointers->m_gta.m_prop_pool)->get_item_count(),
					    (*g_pointers->m_gta.m_prop_pool)->m_size);
			}

			if (g.window.ingame_overlay.show_game_versions)
			{
				ImGui::Separator();
				ImGui::Text("Game Version: %s", g_pointers->m_gta.m_game_version);
				ImGui::Text("Online Version: %s", g_pointers->m_gta.m_online_version);
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, g.window.ingame_overlay.corner == -1))
					g.window.ingame_overlay.corner = -1;
				if (ImGui::MenuItem("Top-left", NULL, g.window.ingame_overlay.corner == 0))
					g.window.ingame_overlay.corner = 0;
				if (ImGui::MenuItem("Top-right", NULL, g.window.ingame_overlay.corner == 1))
					g.window.ingame_overlay.corner = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, g.window.ingame_overlay.corner == 2))
					g.window.ingame_overlay.corner = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, g.window.ingame_overlay.corner == 3))
					g.window.ingame_overlay.corner = 3;
				if (ImGui::MenuItem("Close"))
					g.window.ingame_overlay.opened = false;
				ImGui::EndPopup();
			}
		}
		ImGui::End();

		g_gui->pop_theme_colors();
	}
}
