#include "views/view.hpp"
#include "pointers.hpp"

namespace big
{
	void view::handling_tabs() {
		if (ImGui::BeginTabBar("handling_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_HANDLING_CURRENT_PROFILE"_T.data())) 
			{
				view::handling_current_profile();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_HANDLING_SAVED_PROFILES"_T.data()))
			{
				view::handling_saved_profiles();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
