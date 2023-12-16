/**
 * @file view_debug_locals.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta_util.hpp"
#include "gui/components/components.hpp"
#include "script_local.hpp"
#include "thread_pool.hpp"
#include "views/view.hpp"
#include "widgets/imgui_bitfield.hpp"

namespace big
{
	enum LocalAppendageType : int
	{
		LocalAppendageType_At,
		LocalAppendageType_ReadLocal,
		LocalAppendageType_PlayerId,
	};

	enum LocalValueType : int
	{
		INT,
		BOOLEAN,
		BITSET,
		FLOAT,
		VECTOR,
		VARCHAR
	};

	struct local_debug_inner
	{
		LocalAppendageType type{};
		std::ptrdiff_t index{};
		std::size_t size{};
		std::string local_name{};

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(local_debug_inner, type, index, size, local_name)
	};

	struct local_debug
	{
		std::string script_name{};
		std::size_t local_index{};
		std::vector<local_debug_inner> local_appendages{};
		LocalValueType local_value_type = LocalValueType::INT;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(local_debug, script_name, local_index, local_appendages, local_value_type)
	};

	nlohmann::json get_locals_json()
	{
		nlohmann::json locals{};

		auto file = g_file_manager.get_project_file("./locals.json");
		if (file.exists())
		{
			std::ifstream iffstream_file(file.get_path());
			iffstream_file >> locals;
		}

		return locals;
	}

	void load_local_menu(const std::string& selected_local, local_debug& local_obj)
	{
		if (!selected_local.empty())
		{
			auto locals = get_locals_json();
			if (locals[selected_local].is_null())
				return;
			local_obj = locals[selected_local].get<local_debug>();
		}
	}

	int64_t* get_local_ptr(GtaThread* local_thread, local_debug& local_test)
	{
		if (local_thread == nullptr)
			return nullptr;

		script_local local_to_read = script_local(local_thread, local_test.local_index);
		for (auto item : local_test.local_appendages)
		{
			if (item.type == LocalAppendageType_At)
			{
				if (item.size != 0)
				{
					local_to_read = local_to_read.at(item.index, item.size);
				}
				else
				{
					local_to_read = local_to_read.at(item.index);
				}
			}
			else if (item.type == LocalAppendageType_ReadLocal)
			{
				local_debug local_read;
				load_local_menu(item.local_name, local_read);
				if (auto ptr = (PINT)get_local_ptr(local_thread, local_read))
				{
					auto value = *ptr;
					if (value < 0 || value > INT16_MAX)
					{
						LOG(WARNING) << item.local_name << " was out of bounds for a Read Local.";
						continue;
					}
					if (item.size != 0)
					{
						local_to_read = local_to_read.at(value, item.size);
					}
					else
					{
						local_to_read = local_to_read.at(value);
					}
				}
				else
				{
					LOG(WARNING) << "Failed to read " << item.local_name << "for get_local_ptr";
				}
			}
			else if (item.type == LocalAppendageType_PlayerId)
			{
				if (item.size != 0)
					local_to_read = local_to_read.at(self::id, item.size);
				else
					local_to_read = local_to_read.at(self::id);
			}
		}
		auto retn_val = local_to_read.as<int64_t*>();
		if ((size_t)retn_val < UINT32_MAX)
			return nullptr;
		return retn_val;
	}

	std::string get_local_display(local_debug& local_test)
	{
		auto local_thread = gta_util::find_script_thread(rage::joaat(local_test.script_name));

		if (local_thread == nullptr)
			return "Script does not exist.";

		auto ptr = get_local_ptr(local_thread, local_test);
		if (ptr != nullptr)
		{
			switch (local_test.local_value_type)
			{
			case LocalValueType::INT:
			{
				return std::to_string(*(PINT)ptr);
			}
			case LocalValueType::BOOLEAN:
			{
				return (*ptr == TRUE) ? "TRUE" : "FALSE";
			}
			case LocalValueType::BITSET:
			{
				std::ostringstream o;
				o << "0x" << std::hex << std::uppercase << (DWORD)*ptr;
				return o.str();
			}
			case LocalValueType::FLOAT:
			{
				return std::to_string(*(PFLOAT)ptr);
			}
			case LocalValueType::VECTOR:
			{
				std::ostringstream o;
				auto vectorptr = (rage::scrVector*)ptr;
				o << "X: " << std::fixed << std::setprecision(2) << vectorptr->x << " Y: " << vectorptr->y
				  << " Z: " << vectorptr->z;
				return o.str();
			}
			case LocalValueType::VARCHAR:
			{
				return (PCHAR)ptr;
			}
			}
		}
		return "Invalid Local data read.";
	}

	std::map<std::string, local_debug> list_locals()
	{
		auto json = get_locals_json();
		std::map<std::string, local_debug> return_value;
		for (auto& item : json.items())
			return_value[item.key()] = item.value();
		return return_value;
	}

	void save_local(char* local_name, local_debug& local_obj)
	{
		std::string teleport_name_string = local_name;
		if (!teleport_name_string.empty())
		{
			auto json                  = get_locals_json();
			json[teleport_name_string] = local_obj;

			auto file_path = g_file_manager.get_project_file("./locals.json").get_path();
			std::ofstream file(file_path, std::ios::out | std::ios::trunc);
			file << json.dump(4);
			file.close();
			ZeroMemory(local_name, sizeof(local_name));
		}
	}

	void delete_local(std::string name)
	{
		auto locations = get_locals_json();
		if (locations[name].is_null())
			return;
		locations.erase(name);
		auto file_path = g_file_manager.get_project_file("./locals.json").get_path();
		std::ofstream file(file_path, std::ios::out | std::ios::trunc);
		file << locations.dump(4);
		file.close();
	}

	void view::debug_locals()
	{
		static local_debug local_test{};
		static script_local local_laddie = script_local(local_test.local_index);

		ImGui::SetNextItemWidth(300.f);

		components::input_text("Script Name", local_test.script_name);

		auto local_thread = gta_util::find_script_thread(rage::joaat(local_test.script_name));

		if (local_thread == nullptr)
			ImGui::TextUnformatted("Script does not exist.");

		ImGui::PushItemWidth(200.f);

		ImGui::InputScalar("Local", ImGuiDataType_U16, &local_test.local_index);

		if (local_thread)
			local_laddie = script_local(local_thread, local_test.local_index);
		else
			local_laddie = script_local(local_test.local_index);

		for (int i = 0; i < local_test.local_appendages.size(); i++)
		{
			auto item = local_test.local_appendages[i];
			ImGui::PushID(i + item.type);
			switch (item.type)
			{
			case LocalAppendageType_At:
				ImGui::InputScalar("At", ImGuiDataType_U16, &local_test.local_appendages[i].index);
				ImGui::SameLine();
				ImGui::InputScalar("Size", ImGuiDataType_U16, &local_test.local_appendages[i].size);
				break;
			case LocalAppendageType_ReadLocal:
				ImGui::Text("%s %s", "Read Local", item.local_name.c_str());
				ImGui::SameLine();
				ImGui::InputScalar("Size", ImGuiDataType_U16, &local_test.local_appendages[i].size);
				break;
			case LocalAppendageType_PlayerId:
				ImGui::InputScalar("Read Player ID Size",
				    ImGuiDataType_U16,
				    &local_test.local_appendages[i].size);
				break;
			}
			ImGui::PopID();
		}

		ImGui::PopItemWidth();

		if (ImGui::Button("Add Offset"))
			local_test.local_appendages.push_back({LocalAppendageType_At, 0LL, 0ULL});
		ImGui::SameLine();
		if (ImGui::Button("Add Read Player Id"))
			local_test.local_appendages.push_back({LocalAppendageType_PlayerId, 0LL, 0ULL});

		if (local_test.local_appendages.size() > 0 && ImGui::Button("Remove Offset"))
			local_test.local_appendages.pop_back();

		if (auto ptr = get_local_ptr(local_thread, local_test))
		{
			switch (local_test.local_value_type)
			{
			case LocalValueType::INT:
			{
				ImGui::InputScalar("Value", ImGuiDataType_S32, ptr);
				break;
			}
			case LocalValueType::BOOLEAN:
			{
				bool is_local_enabled = (*ptr == TRUE);
				if (ImGui::Checkbox("Value", &is_local_enabled))
				{
					*ptr = is_local_enabled;
				}
				break;
			}
			case LocalValueType::BITSET:
			{
				ImGui::Bitfield("Value", (PINT)ptr);
				break;
			}
			case LocalValueType::FLOAT:
			{
				ImGui::InputScalar("Value", ImGuiDataType_Float, ptr);
				break;
			}
			case LocalValueType::VECTOR:
			{
				ImGui::PushItemWidth(100.f);
				auto vectorptr = (rage::scrVector*)ptr;
				ImGui::InputScalar("X", ImGuiDataType_Float, &vectorptr->x);
				ImGui::SameLine();
				ImGui::InputScalar("Y", ImGuiDataType_Float, &vectorptr->y);
				ImGui::SameLine();
				ImGui::InputScalar("Z", ImGuiDataType_Float, &vectorptr->z);
				ImGui::PopItemWidth();
				break;
			}
			case LocalValueType::VARCHAR:
			{
				std::string characters = (PCHAR)ptr;
				try
				{
					components::input_text("Value", (PCHAR)ptr, 255);
				}
				catch (...)
				{
				} //This can crash if the user tries to edit the invalid ??? scenario from ImGui, so to prevent that, just silently do nothing.
				break;
			}
			}
		}
		else
		{
			ImGui::TextUnformatted("Invalid Local data read.");
		}

		ImGui::PopItemWidth();

		ImGui::SetNextItemWidth(150.f);
		ImGui::Combo("Type", (int*)&local_test.local_value_type, "INT\0BOOLEAN\0BITSET\0FLOAT\0VECTOR\0VARCHAR\0");

		auto locals = list_locals();
		static std::string selected_local;
		ImGui::TextUnformatted("Saved Locals");
		if (ImGui::BeginListBox("##savedlocals", ImVec2(200, 250)))
		{
			for (auto pair : locals)
			{
				if (ImGui::Selectable(pair.first.c_str(), selected_local == pair.first))
					selected_local = std::string(pair.first);
			}
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		if (ImGui::BeginListBox("##localvalues", ImVec2(250, 250)))
		{
			for (auto pair : locals)
			{
				ImGui::Selectable(get_local_display(pair.second).c_str(), false, ImGuiSelectableFlags_Disabled);
			}
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		ImGui::BeginGroup();
		static char local_name[50]{};
		ImGui::SetNextItemWidth(200.f);
		components::input_text("##localname", local_name, IM_ARRAYSIZE(local_name));
		if (ImGui::IsItemActive())
			g.self.hud.typing = TYPING_TICKS;
		if (ImGui::Button("Save Local"))
		{
			save_local(local_name, local_test);
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Local"))
		{
			load_local_menu(selected_local, local_test);
		}

		if (ImGui::Button("Delete Local"))
		{
			if (!selected_local.empty())
			{
				delete_local(selected_local);
				selected_local.clear();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Read Local"))
		{
			local_debug local_read{};
			load_local_menu(selected_local, local_read);
			if (local_read.local_value_type == LocalValueType::INT)
			{
				local_test.local_appendages.push_back({LocalAppendageType_ReadLocal, 0LL, 0ULL, selected_local});
			}
			else
			{
				g_notification_service->push_warning("Locals", "You cannot use Read Local with a non-integer type.");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			local_test.local_index = 0;
			local_test.local_appendages.clear();
		}
		ImGui::EndGroup();
	}
}