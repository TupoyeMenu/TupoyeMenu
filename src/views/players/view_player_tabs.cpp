#include "pointers.hpp"
#include "services/players/player_service.hpp"
#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::view_player_tabs()
	{
		if (!*g_pointers->m_gta.m_is_session_started && !g_player_service->get_selected()->is_valid())
			return;

		if (ImGui::Begin("Player", &g.window.player))
		{
			if (ImGui::BeginTabBar("player_tabbar"))
			{
				if (ImGui::BeginTabItem("INFO"_T.data()))
				{
					view::view_player_info();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("INFO"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("TROLL"_T.data()))
				{
					view::view_player_troll();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("TROLL"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("VEHICLE"_T.data()))
				{
					view::view_player_vehicle();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("VEHICLE"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("MISC"_T.data()))
				{
					view::view_player_misc();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("MISC"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("TOXIC"_T.data()))
				{
					view::view_player_toxic();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("TOXIC"));
					ImGui::EndTabItem();
				}

#ifdef ENABLE_CRASHES
				if (ImGui::BeginTabItem("Kicks and Crashes"))
#else
				if (ImGui::BeginTabItem("Kicks"))
#endif // ENABLE_CRASHES
				{
					view::view_player_kicks_and_crashes();
					if (g_lua_manager)
						g_lua_manager->draw_gui(RAGE_JOAAT("KICK"));
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}
}
