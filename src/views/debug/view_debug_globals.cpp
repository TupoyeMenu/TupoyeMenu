/**
 * @file view_debug_globals.cpp
 * @brief In game script global editor.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gui/components/components.hpp"
#include "script_global.hpp"
#include "thread_pool.hpp"
#include "views/view.hpp"
#include "widgets/imgui_bitfield.hpp"

namespace big
{
	enum GlobalAppendageType : int
	{
		GlobalAppendageType_At,
		GlobalAppendageType_ReadGlobal,
		GlobalAppendageType_PlayerId,
	};

	enum GlobalValueType : int
	{
		INT,
		BOOLEAN,
		BITSET,
		FLOAT,
		VECTOR,
		VARCHAR
	};

	struct global_debug_inner
	{
		GlobalAppendageType type{};
		std::ptrdiff_t index{};
		std::size_t size{};
		std::string global_name{};

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(global_debug_inner, type, index, size, global_name)
	};

	struct global_debug
	{
		std::size_t global_index{};
		std::vector<global_debug_inner> global_appendages{};
		GlobalValueType global_value_type = GlobalValueType::INT;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(global_debug, global_index, global_appendages, global_value_type)
	};

	nlohmann::json get_globals_json()
	{
		nlohmann::json globals{};

		auto file = g_file_manager.get_project_file("./globals.json");
		if (file.exists())
		{
			std::ifstream iffstream_file(file.get_path());
			iffstream_file >> globals;
		}

		return globals;
	}

	void load_global_menu(const std::string& selected_global, global_debug& global_obj)
	{
		if (!selected_global.empty())
		{
			auto globals = get_globals_json();
			if (globals[selected_global].is_null())
				return;
			global_obj = globals[selected_global].get<global_debug>();
		}
	}

	int64_t* get_global_ptr(global_debug& global_test)
	{
		script_global global_to_read = script_global(global_test.global_index);
		for (auto item : global_test.global_appendages)
		{
			if (item.type == GlobalAppendageType_At)
			{
				if (item.size != 0)
					global_to_read = global_to_read.at(item.index, item.size);
				else
					global_to_read = global_to_read.at(item.index);
			}
			else if (item.type == GlobalAppendageType_ReadGlobal)
			{
				global_debug global_read;
				load_global_menu(item.global_name, global_read);
				if (auto ptr = get_global_ptr(global_read))
					if (item.size != 0)
						global_to_read = global_to_read.at(*ptr, item.size);
					else
						global_to_read = global_to_read.at(*ptr);
				else
					LOG(WARNING) << "Failed to read " << item.global_name << "for get_global_ptr";
			}
			else if (item.type == GlobalAppendageType_PlayerId)
			{
				if (item.size != 0)
					global_to_read = global_to_read.at(self::id, item.size);
				else
					global_to_read = global_to_read.at(self::id);
			}
		}
		auto retn_val = global_to_read.as<int64_t*>();
		if ((size_t)retn_val < UINT32_MAX)
			return nullptr;
		return retn_val;
	}

	std::string get_global_display(global_debug& global_test)
	{
		auto ptr = get_global_ptr(global_test);
		if (ptr != nullptr)
		{
			switch (global_test.global_value_type)
			{
				case GlobalValueType::INT:
				{
					return std::to_string(*(PINT)ptr);
				}
				case GlobalValueType::BOOLEAN:
				{
					return (*ptr == TRUE) ? "TRUE" : "FALSE";
				}
				case GlobalValueType::BITSET:
				{
					std::ostringstream o;
					o << "0x" << std::hex << std::uppercase << (DWORD)*ptr;
					return o.str();
				}
				case GlobalValueType::FLOAT:
				{
					return std::to_string(*(PFLOAT)ptr);
				}
				case GlobalValueType::VECTOR:
				{
				    std::ostringstream o;
				    auto vectorptr = (rage::scrVector*)ptr;
				    o << "X: " << std::fixed << std::setprecision(2) << vectorptr->x << " Y: " << vectorptr->y << " Z: " << vectorptr->z;
				    return o.str();
				}
				case GlobalValueType::VARCHAR:
				{
					return (PCHAR)ptr;
				}
			}
		}
		return "Invalid Global data read.";
	}

	std::map<std::string, global_debug> list_globals()
	{
		auto json = get_globals_json();
		std::map<std::string, global_debug> return_value;
		for (auto& item : json.items())
			return_value[item.key()] = item.value();
		return return_value;
	}

	void save_global(char* global_name, global_debug& global_obj)
	{
		std::string teleport_name_string = global_name;
		if (!teleport_name_string.empty())
		{
			auto json                  = get_globals_json();
			json[teleport_name_string] = global_obj;

			auto file_path = g_file_manager.get_project_file("./globals.json").get_path();
			std::ofstream file(file_path, std::ios::out | std::ios::trunc);
			file << json.dump(4);
			file.close();
			ZeroMemory(global_name, sizeof(global_name));
		}
	}

	void delete_global(std::string name)
	{
		auto locations = get_globals_json();
		if (locations[name].is_null())
			return;
		locations.erase(name);
		auto file_path = g_file_manager.get_project_file("./globals.json").get_path();
		std::ofstream file(file_path, std::ios::out | std::ios::trunc);
		file << locations.dump(4);
		file.close();
	}

	void view::debug_globals()
	{
		static global_debug global_test{};
		static script_global glo_bal_sunday = script_global(global_test.global_index);
		ImGui::SetNextItemWidth(200.f);
		if (ImGui::InputScalar("Global", ImGuiDataType_U64, &global_test.global_index))
			glo_bal_sunday = script_global(global_test.global_index);

		for (int i = 0; i < global_test.global_appendages.size(); i++)
		{
			static global_debug global_test{};
			static script_global glo_bal_sunday = script_global(global_test.global_index);
			ImGui::SetNextItemWidth(200.f);
			if (ImGui::InputScalar("Global", ImGuiDataType_U32, &global_test.global_index))
				glo_bal_sunday = script_global(global_test.global_index);

			for (int i = 0; i < global_test.global_appendages.size(); i++)
			{
				auto item = global_test.global_appendages[i];
				ImGui::PushID(i + item.type);
				switch (item.type)
				{
					case GlobalAppendageType_At:
						ImGui::SetNextItemWidth(200.f);
						ImGui::InputScalar("At", ImGuiDataType_U16, &global_test.global_appendages[i].index);
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200.f);
						ImGui::InputScalar("Size", ImGuiDataType_U16, &global_test.global_appendages[i].size);
						break;
					case GlobalAppendageType_ReadGlobal:
						ImGui::Text(std::format("{} {}", "Read Global", item.global_name).c_str());
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200.f);
						ImGui::InputScalar("Size", ImGuiDataType_U16, &global_test.global_appendages[i].size);
						break;
					case GlobalAppendageType_PlayerId:
						ImGui::SetNextItemWidth(200.f);
						ImGui::InputScalar("Read Player ID Size", ImGuiDataType_U16, &global_test.global_appendages[i].size);
						break;
				}
				ImGui::PopID();
			}

			if (ImGui::Button("Add Offset"))
				global_test.global_appendages.push_back({GlobalAppendageType_At, 0LL, 0ULL});
			ImGui::SameLine();
			if (ImGui::Button("Add Read Player Id"))
				global_test.global_appendages.push_back({GlobalAppendageType_PlayerId, 0LL, 0ULL});

			if (global_test.global_appendages.size() > 0 && ImGui::Button("Remove Offset"))
				global_test.global_appendages.pop_back();

			if (auto ptr = get_global_ptr(global_test))
			{
				switch (global_test.global_value_type)
				{
					case GlobalValueType::INT:
					{
					    ImGui::SetNextItemWidth(300.f);
					    ImGui::InputScalar("Value", ImGuiDataType_S32, ptr);
					    break;
					}
				    case GlobalValueType::BOOLEAN:
				    {
					    ImGui::SetNextItemWidth(300.f);
					    bool is_global_enabled = (*ptr == TRUE);
					    if (ImGui::Checkbox("Value", &is_global_enabled))
					    {
					        *ptr = is_global_enabled;
					    }
					    break;
				    }
				    case GlobalValueType::BITSET:
				    {
					    ImGui::SetNextItemWidth(300.f);
					    ImGui::Bitfield("Value", (PINT)ptr);
					    break;
				    }
				    case GlobalValueType::FLOAT:
					{
					    ImGui::SetNextItemWidth(300.f);
					    ImGui::InputScalar("Value", ImGuiDataType_Float, ptr);
					    break;
					}
				    case GlobalValueType::VECTOR:
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
				    case GlobalValueType::VARCHAR:
					{
					    ImGui::SetNextItemWidth(300.f);
					    std::string characters = (PCHAR)ptr;
						try
						{
						    components::input_text("Value", (PCHAR)ptr, 255);
						} catch (...){ } //This can crash if the user tries to edit the invalid ??? scenario from ImGui, so to prevent that, just silently do nothing.
					    break;
					}
				}
			}
			else
			{
				ImGui::Text("Invalid Global data read.");
			}

			ImGui::SetNextItemWidth(150.f);
			ImGui::Combo("Type", (int*)&global_test.global_value_type, "INT\0BOOLEAN\0BITSET\0FLOAT\0VECTOR\0VARCHAR\0");

			auto globals = list_globals();
			static std::string selected_global;
			ImGui::Text("Saved Globals");
			if (ImGui::BeginListBox("##savedglobals", ImVec2(200, 250)))
			{
				for (auto pair : globals)
				{
					if (ImGui::Selectable(pair.first.c_str(), selected_global == pair.first))
						selected_global = std::string(pair.first);
				}
				ImGui::EndListBox();
			}
			ImGui::SameLine();
			if (ImGui::BeginListBox("##globalvalues", ImVec2(250, 250)))
			{
				for (auto pair : globals)
				{
					ImGui::Selectable(get_global_display(pair.second).c_str(), false, ImGuiSelectableFlags_Disabled);
				}
				ImGui::EndListBox();
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			static char global_name[50]{};
			ImGui::SetNextItemWidth(200.f);
			components::input_text("##GlobalName", global_name, IM_ARRAYSIZE(global_name));
			if (ImGui::IsItemActive())
				g.self.hud.typing = TYPING_TICKS;
			if (ImGui::Button("Save Global"))
			{
				save_global(global_name, global_test);
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Global"))
			{
				load_global_menu(selected_global, global_test);
			}

			if (ImGui::Button("Delete Global"))
			{
				if (!selected_global.empty())
				{
					delete_global(selected_global);
					selected_global.clear();
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Add Read Global"))
			{
				global_debug global_read{};
				load_global_menu(selected_global, global_read);
				if (global_read.global_value_type == GlobalValueType::INT)
				{
					global_test.global_appendages.push_back({GlobalAppendageType_ReadGlobal, 0LL, 0ULL, selected_global});
				}
				else
				{
					g_notification_service->push_warning("Globals", "You cannot use Read Global with a non-integer type.");
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				global_test.global_index = 0;
				global_test.global_appendages.clear();
			}
			ImGui::EndGroup();
			ImGui::EndTabItem();
		}

		if (ImGui::Button("Add Offset"))
			global_test.global_appendages.push_back({GlobalAppendageType_At, 0LL, 0ULL});
		ImGui::SameLine();
		if (ImGui::Button("Add Read Player Id"))
			global_test.global_appendages.push_back({GlobalAppendageType_PlayerId, 0LL, 0ULL});

		if (global_test.global_appendages.size() > 0 && ImGui::Button("Remove Offset"))
			global_test.global_appendages.pop_back();

		if (auto ptr = get_global_ptr(global_test))
		{
			ImGui::SetNextItemWidth(200.f);
			ImGui::InputScalar("Value", ImGuiDataType_S32, ptr);
		}
		else
			ImGui::Text("INVALID_GLOBAL_READ");

		auto globals = list_globals();
		static std::string selected_global;
		ImGui::Text("Saved Globals");
		if (ImGui::BeginListBox("##savedglobals", ImVec2(200, 200)))
		{
			for (auto pair : globals)
			{
				if (ImGui::Selectable(pair.first.c_str(), selected_global == pair.first))
					selected_global = std::string(pair.first);
			}
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		if (ImGui::BeginListBox("##globalvalues", ImVec2(200, 200)))
		{
			for (auto pair : globals)
			{
				if (auto ptr = get_global_ptr(pair.second))
					ImGui::Selectable(std::format("{}", (std::int32_t)*ptr).c_str(), false, ImGuiSelectableFlags_Disabled);
				else
					ImGui::Selectable("INVALID_GLOBAL_READ", false, ImGuiSelectableFlags_Disabled);
			}
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		ImGui::BeginGroup();
		static char global_name[50]{};
		ImGui::SetNextItemWidth(200.f);
		ImGui::InputText("##GlobalName", global_name, IM_ARRAYSIZE(global_name));
		if (ImGui::IsItemActive())
			g.self.hud.typing = TYPING_TICKS;

		if (ImGui::Button("Save Global"))
		{
			save_global(global_name, global_test);
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Global"))
		{
			load_global_menu(selected_global, global_test);
		}

		if (ImGui::Button("Delete Global"))
		{
			if (!selected_global.empty())
			{
				delete_global(selected_global);
				selected_global.clear();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Read Global"))
		{
			global_test.global_appendages.push_back({GlobalAppendageType_ReadGlobal, 0LL, 0ULL, selected_global});
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			global_test.global_index = 0;
			global_test.global_appendages.clear();
		}
		ImGui::EndGroup();
	}
}