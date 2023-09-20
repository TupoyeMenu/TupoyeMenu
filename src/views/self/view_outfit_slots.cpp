/**
 * @file view_outfit_slots.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "natives.hpp"
#include "pointers.hpp"
#include "util/outfit.hpp"
#include "views/view.hpp"

namespace big
{
	void view::outfit_slots()
	{
		if (*g_pointers->m_gta.m_script_globals)
		{
			static int slot = 0;
			ImGui::SetNextItemWidth(160);
			if (ImGui::InputInt("Slot", &slot))
			{
				if (slot < 0)
					slot = 19;
				if (slot > 19)
					slot = 0;
			}

			ImGui::SetNextItemWidth(300);
			ImGui::InputText("Name", outfit::get_slot_name_address(slot), 16);
			if (ImGui::IsItemActive())
				g.self.hud.typing = TYPING_TICKS;

			static outfit::components_t components;
			static outfit::props_t props;

			g_fiber_pool->queue_job([] {
				for (auto& item : components.items)
				{
					item.drawable_id     = *outfit::get_component_drawable_id_address(slot, item.id);
					item.drawable_id_max = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(self::ped, item.id) - 1;

					item.texture_id = *outfit::get_component_texture_id_address(slot, item.id);
					item.texture_id_max = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(self::ped, item.id, item.drawable_id) - 1;
				}

				for (auto& item : props.items)
				{
					item.drawable_id     = *outfit::get_prop_drawable_id_address(slot, item.id);
					item.drawable_id_max = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(self::ped, item.id) - 1;

					item.texture_id = *outfit::get_prop_texture_id_address(slot, item.id);
					item.texture_id_max = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(self::ped, item.id, item.drawable_id) - 1;
				}
			});

			components::button("Export To Clipboard", [] {
				std::stringstream ss;
				for (auto& item : components.items)
					ss << item.id << " " << item.drawable_id << " " << item.texture_id << " ";
				for (auto& item : props.items)
					ss << item.id << " " << item.drawable_id << " " << item.texture_id << " ";
				ImGui::SetClipboardText(ss.str().c_str());
				g_notification_service->push_success("Outfit", "Export To Clipboard");
			});
			ImGui::SameLine();

			components::button("Import From Clipboard", [] {
				std::stringstream ss(ImGui::GetClipboardText());
				for (auto& item : components.items)
				{
					int id          = 0;
					int drawable_id = 0;
					int texture_id  = 0;
					ss >> id;
					ss >> drawable_id;
					ss >> texture_id;
					*outfit::get_component_drawable_id_address(slot, id) = drawable_id;
					*outfit::get_component_texture_id_address(slot, id)  = texture_id;
				}
				for (auto& item : props.items)
				{
					int id          = 0;
					int drawable_id = 0;
					int texture_id  = 0;
					ss >> id;
					ss >> drawable_id;
					ss >> texture_id;
					*outfit::get_prop_drawable_id_address(slot, id) = drawable_id;
					*outfit::get_prop_texture_id_address(slot, id)  = texture_id;
				}
			});

			ImGui::BeginGroup();
			for (auto& item : components.items)
			{
				ImGui::SetNextItemWidth(120);
				ImGui::InputInt(std::format("{} [0,{}]##1", item.label, item.drawable_id_max).c_str(), outfit::get_component_drawable_id_address(slot, item.id));
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			for (auto& item : components.items)
			{
				ImGui::SetNextItemWidth(120);
				ImGui::InputInt(std::format("{} {} [0,{}]##2", item.label, "TEX", item.texture_id_max).c_str(), outfit::get_component_texture_id_address(slot, item.id));
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			for (auto& item : props.items)
			{
				ImGui::SetNextItemWidth(120);
				ImGui::InputInt(std::format("{} [0,{}]##3", item.label, item.drawable_id_max).c_str(), outfit::get_prop_drawable_id_address(slot, item.id));
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			for (auto& item : props.items)
			{
				ImGui::SetNextItemWidth(120);
				ImGui::InputInt(std::format("{} {} [0,{}]##4", item.label, "TEX", item.texture_id_max).c_str(), outfit::get_prop_texture_id_address(slot, item.id));
			}
			ImGui::EndGroup();
		}
	}
}