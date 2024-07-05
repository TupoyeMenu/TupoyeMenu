/**
 * @file view_world_tabs.cpp
 * @brief World options tab bar.
 */

#include "lua/lua_manager.hpp"
#include "views/view.hpp"

namespace big
{
	void view::world_tabs()
	{
		if (ImGui::BeginTabBar("world_tabbar"))
		{
			if(ImGui::BeginTabItem("World"))
			{
				view::world();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_WORLD"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spawn Ped"))
			{
				view::spawn_ped();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_SPAWN_PED"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Embient Population"))
			{
				view::ambient();
				if (g_lua_manager)
					g_lua_manager->draw_gui("Embient Population"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Creator"))
			{
				view::creator();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_CREATOR"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Blackhole"))
			{
				view::blackhole();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_BLACKHOLE"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Model Swapper"))
			{
				view::model_swapper();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_MODEL_SWAPPER"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("VFX"))
			{
				view::vfx();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_VFX"_J);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("XML"))
			{
				view::xml_maps();
				if (g_lua_manager)
					g_lua_manager->draw_gui("GUI_TAB_XML_MAPS"_J);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
