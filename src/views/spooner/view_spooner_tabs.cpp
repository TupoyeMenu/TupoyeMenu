#include "views/view.hpp"
#include "pointers.hpp"
#include "services/object_spooner/object_spooner_service.hpp"

namespace big
{
	void view::spooner_tabs() {
		if(g_object_spooner_service->selected_entity.m_does_entity_exist)
		{
			if(ImGui::Begin("Object Spooner"))
			{
				if (ImGui::BeginTabBar("spooner_tabbar"))
				{
					if (ImGui::BeginTabItem("Info"))
					{
						view::spooner_info();
						ImGui::EndTabItem();
					}

					if(g_object_spooner_service->selected_entity.m_is_entity_a_ped)
					{
						if (ImGui::BeginTabItem("Outfit Editor"))
						{
							view::outfit_editor(g_object_spooner_service->selected_entity.m_entity_handle);
							ImGui::EndTabItem();
						}
					}
					ImGui::EndTabBar();
				}
				ImGui::End();
			}
		}
	}
}
