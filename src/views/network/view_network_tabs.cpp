#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::network_tabs()
	{
		if (ImGui::BeginTabBar("network_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_SPOOFING"_T.data()))
			{
				view::spoofing();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SESSION"_T.data()))
			{
				view::session();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_MISSIONS"_T.data()))
			{
				view::missions();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_PLAYER_DB"_T.data()))
			{
				view::player_database();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SESSION_BROWSER"_T.data()))
			{
				view::session_browser();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
}
