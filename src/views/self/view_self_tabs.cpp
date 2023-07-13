#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::self_tabs()
	{
		if (ImGui::BeginTabBar("self_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_SELF"_T.data()))
			{
				view::self();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_SELF"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_WEAPONS"_T.data()))
			{
				view::weapons();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_WEAPONS"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_MOBILE"_T.data()))
			{
				view::mobile();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_MOBILE"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_TELEPORT"_T.data()))
			{
				view::teleport();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_TELEPORT"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_CUSTOM_TELEPORT"_T.data()))
			{
				view::custom_teleport;
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_CUSTOM_TELEPORT"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_OUTFIT_EDITOR"_T.data()))
			{
				view::outfit_editor();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_OUTFIT_EDITOR"));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_OUTFIT_SLOTS"_T.data()))
			{
				view::outfit_slots();
				if (g_lua_manager)
					g_lua_manager->draw_gui(RAGE_JOAAT("GUI_TAB_OUTFIT_SLOTS"));
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
