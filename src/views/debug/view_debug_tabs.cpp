#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::debug_tabs()
	{
		if (ImGui::BeginTabBar("debug_tabbar"))
		{
			if (ImGui::BeginTabItem("Threads"))
			{
				view::debug_threads();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Script Events"))
			{
				view::debug_script_events();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Globals"))
			{
				view::debug_globals();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				view::debug_misc();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
