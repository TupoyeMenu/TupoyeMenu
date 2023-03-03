#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::world_tabs()
	{
		if (ImGui::BeginTabBar("world_tabbar"))
		{
			if (ImGui::BeginTabItem("Spawn Ped"))
			{
				view::spawn_ped();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Time and Weather"))
			{
				view::time_and_weather();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Embient Population"))
			{
				view::ambient();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Creator"))
			{
				view::creator();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Train"))
			{
				view::train();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Water"))
			{
				view::water();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Blackhole"))
			{
				view::blackhole();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Model Swapper"))
			{
				view::model_swapper();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Nearby"))
			{
				view::nearby();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
}
