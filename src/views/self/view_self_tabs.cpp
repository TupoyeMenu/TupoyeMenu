#include "pointers.hpp"
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
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_WEAPONS"_T.data()))
			{
				view::weapons();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_MOBILE"_T.data()))
			{
				view::mobile();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_TELEPORT"_T.data()))
			{
				view::teleport();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Outfit Editor"))
			{
				view::outfit_editor(self::ped);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
