/**
 * @file view_context_menu_settings.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "imgui.h"
#include "services/context_menu/context_menu_service.hpp"
#include "views/view.hpp"

namespace big
{
	void view::context_menu_settings()
	{
		ImGui::Checkbox("Context Menu Enabled", &g.context_menu.enabled);

		ImGui::BeginDisabled(!g.context_menu.enabled);
		ImGui::Text("Allowed Entity Types:");
		ImGui::CheckboxFlags("Object", reinterpret_cast<int*>(&g.context_menu.allowed_entity_types), static_cast<int>(ContextEntityType::OBJECT));
		ImGui::SameLine();
		ImGui::CheckboxFlags("Ped", reinterpret_cast<int*>(&g.context_menu.allowed_entity_types), static_cast<int>(ContextEntityType::PED));
		ImGui::SameLine();
		ImGui::CheckboxFlags("Player", reinterpret_cast<int*>(&g.context_menu.allowed_entity_types), static_cast<int>(ContextEntityType::PLAYER));
		ImGui::SameLine();
		ImGui::CheckboxFlags("Vehicle", reinterpret_cast<int*>(&g.context_menu.allowed_entity_types), static_cast<int>(ContextEntityType::VEHICLE));

		static ImVec4 selected_option_color = ImGui::ColorConvertU32ToFloat4(g.context_menu.selected_option_color);
		ImGui::Text("Selected Option Color:");
		if (ImGui::ColorEdit4("###BSelected Option Color##cm_picker", (float*)&selected_option_color, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
		{
			g.context_menu.selected_option_color = ImGui::ColorConvertFloat4ToU32(selected_option_color);
		}

		ImGui::Checkbox("Bounding Box Enabled", &g.context_menu.bounding_box_enabled);

		if (g.context_menu.bounding_box_enabled)
		{
			static ImVec4 bounding_box_color = ImGui::ColorConvertU32ToFloat4(g.context_menu.bounding_box_color);
			ImGui::Text("Bounding Box Color:");
			if (ImGui::ColorEdit4("###Bounding Box Color##cm_picker", (float*)&bounding_box_color, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.context_menu.bounding_box_color = ImGui::ColorConvertFloat4ToU32(bounding_box_color);
			}
		}

		ImGui::EndDisabled();
	}
}
