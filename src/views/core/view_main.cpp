#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::main_view()
	{
		if (ImGui::Begin("Main", &g.window.main))
		{
			if (ImGui::BeginTabBar("tabbar"))
			{
				if (ImGui::BeginTabItem("GUI_TAB_SELF"_T.data()))
				{
					view::self_tabs();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("GUI_TAB_VEHICLE"_T.data()))
				{
					view::vehicle_tabs();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("GUI_TAB_WORLD"_T.data()))
				{
					view::world_tabs();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("GUI_TAB_NETWORK"_T.data()))
				{
					view::network_tabs();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("GUI_TAB_SETTINGS"_T.data()))
				{
					view::settings_tabs();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("GUI_TAB_DEBUG"_T.data()))
				{
					view::debug_tabs();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
	}
}
