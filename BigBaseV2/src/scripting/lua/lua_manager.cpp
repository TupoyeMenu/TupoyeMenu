#include "common.hpp"
#include "lua_manager.hpp"
#include "imgui.h"

namespace big
{
	lua_manager::lua_manager()
	{
		if (!g_file_manager->get_project_file("./scripts").exists())
			std::filesystem::create_directory(g_file_manager->get_project_file("./scripts").get_path());

		if (!g_file_manager->get_project_file("./scripts/lua").exists())
			std::filesystem::create_directory(g_file_manager->get_project_file("./scripts/lua").get_path());

		for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(g_file_manager->get_project_folder("./scripts/lua").get_path()))
		{
			if (dir_entry.is_regular_file())
			{
				const auto& file_path = dir_entry.path();

				if (file_path.filename() == "natives.lua")
					continue;

				if (file_path.extension() == ".lua")
				{
					m_scripts.push_back(std::make_unique<lua_script>(file_path));
				}
			}
		}

		g_lua_manager = this;
	}

	lua_manager::~lua_manager()
	{
		m_scripts.clear();
	}

	void lua_manager::draw_script_gui(std::uint32_t hash)
	{
		for (auto& script : m_scripts)
		{
			if (script->m_script_gui.contains(hash))
			{
				ImGui::Separator();

				for (auto& item : script->m_script_gui[hash])
				{
					item->draw();
				}
			}
		}
	}
}