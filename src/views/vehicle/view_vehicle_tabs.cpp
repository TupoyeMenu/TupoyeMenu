/**
 * @file view_vehicle_tabs.cpp
 * @brief Vehicle options tab bar.
 */

#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::vehicle_tabs()
	{
		if (ImGui::BeginTabBar("vehicle_tabbar"))
		{
			if (ImGui::BeginTabItem("Vehicle"))
			{
				view::vehicle();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_VEHICLE"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("LS Customs"))
			{
				view::lsc();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_LSC"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spawn Vehicle"))
			{
				view::spawn_vehicle();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_SPAWN_VEHICLE"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Fun Features"))
			{
				view::fun_vehicle();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_VEHICLE_FUN_FEATURES"_J);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
