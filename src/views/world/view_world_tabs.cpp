#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::world_tabs()
	{
		if (ImGui::BeginTabBar("world_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_WORLD"_T.data()))
			{
				view::world();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_WORLD"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SPAWN_PED"_T.data()))
			{
				view::spawn_ped();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SPAWN_PED"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Squad spawner"))
			{
				view::squad_spawner();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Squad spawner"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Embient Population"))
			{
				view::ambient();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("Embient Population"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_CREATOR"_T.data()))
			{
				view::creator();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_CREATOR"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_TRAIN"_T.data()))
			{
				view::train();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_TRAIN"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_BLACKHOLE"_T.data()))
			{
				view::blackhole();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_BLACKHOLE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_MODEL_SWAPPER"_T.data()))
			{
				view::model_swapper();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_MODEL_SWAPPER"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_VFX"_T.data()))
			{
				view::vfx();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_VFX"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
