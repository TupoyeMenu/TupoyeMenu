#include "views/view.hpp"
#include "pointers.hpp"

namespace big
{
	void view::debug_tabs() {
		if (ImGui::BeginTabBar("debug_tabbar"))
		{
			if (ImGui::BeginTabItem("DEBUG_TAB_SCRIPTS"_T.data()))
			{
				view::debug_scripts();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG_TAB_SCRIPT_EVENTS"_T.data()))
			{
				view::debug_script_events();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG_TAB_GLOBALS"_T.data()))
			{
				view::debug_globals();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Ped Flags"))
			{
				view::debug_ped_flags();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("DEBUG_TAB_MISC"_T.data()))
			{
				view::debug_misc();
				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}
	}
}
