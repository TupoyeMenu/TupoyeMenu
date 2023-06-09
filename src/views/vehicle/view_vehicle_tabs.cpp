#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::vehicle_tabs()
	{
		if (ImGui::BeginTabBar("vehicle_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_VEHICLE"_T.data()))
			{
				view::vehicle();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_VEHICLE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_HANDLING"_T.data()))
			{
				view::handling_tabs();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_HANDLING"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_LSC"_T.data()))
			{
				view::lsc();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_LSC"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SPAWN_VEHICLE"_T.data()))
			{
				view::spawn_vehicle();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SPAWN_VEHICLE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PERSONAL_VEHICLE"_T.data()))
			{
				view::pv();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_PERSONAL_VEHICLE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PERSIST_CAR"_T.data()))
			{
				view::persist_car();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_PERSIST_CAR"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_VEHICLE_FUN_FEATURES"_T.data()))
			{
				view::fun_vehicle();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_VEHICLE_FUN_FEATURES"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
