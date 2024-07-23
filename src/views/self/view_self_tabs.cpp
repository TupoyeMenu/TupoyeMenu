/**
 * @file view_self_tabs.cpp
 * @brief Self options tab bar.
 */

#include "lua/lua_manager.hpp"
#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::self_tabs()
	{
		if (ImGui::BeginTabBar("self_tabbar"))
		{
			if (ImGui::BeginTabItem("Self"))
			{
				view::self();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_SELF"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Weapons"))
			{
				view::weapons();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_WEAPONS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Mobile"))
			{
				view::mobile();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_MOBILE"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Teleport"))
			{
				view::teleport();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_TELEPORT"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Custom Teleport"))
			{
				view::custom_teleport();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_CUSTOM_TELEPORT"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Outfit Editor"))
			{
				view::outfit_editor();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_OUTFIT_EDITOR"_J);
				ImGui::EndTabItem();
			}
			if (*g_pointers->m_gta.m_script_globals && ImGui::BeginTabItem("Outfit Slots"))

			{
				view::outfit_slots();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_OUTFIT_SLOTS"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Animations"))
			{
				view::animations();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_ANIMATIONS"_J);
				ImGui::EndTabItem();
			}

			if (g_lua_manager)
				g_lua_manager->draw_child_tabs_external("GUI_TAB_SELF"_J);

			ImGui::EndTabBar();
		}
	}
}
