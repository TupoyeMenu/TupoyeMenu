#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::world_tabs()
	{
		if (ImGui::BeginTabBar("world_tabbar"))
		{
			if (ImGui::BeginTabItem("GUI_TAB_WORLD"_T.data()))
			{
				view::world();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_SPAWN_PED"_T.data()))
			{
				view::spawn_ped();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Embient Population"))
			{
				view::ambient();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_CREATOR"_T.data()))
			{
				view::creator();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_TRAIN"_T.data()))
			{
				view::train();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_BLACKHOLE"_T.data()))
			{
				view::blackhole();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_MODEL_SWAPPER"_T.data()))
			{
				view::model_swapper();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_NEARBY"_T.data()))
			{
				view::nearby();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("GUI_TAB_ORBITAL_DRONE"_T.data()))
			{
				view::orbital_drone();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
}
