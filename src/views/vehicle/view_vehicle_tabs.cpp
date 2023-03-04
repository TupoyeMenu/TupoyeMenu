#include "pointers.hpp"
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
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_HANDLING"_T.data()))
			{
				view::handling_tabs();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_LSC"_T.data()))
			{
				view::lsc();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SPAWN_VEHICLE"_T.data()))
			{
				view::spawn_vehicle();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PERSONAL_VEHICLE"_T.data()))
			{
				view::pv();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PERSIST_CAR"_T.data()))
			{
				view::persist_car();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_VEHICLE_FUN_FEATURES"_T.data()))
			{
				view::fun_vehicle();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
