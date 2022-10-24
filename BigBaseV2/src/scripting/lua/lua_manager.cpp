#include "common.hpp"
#include "lua_manager.hpp"

namespace big
{
	lua_manager::lua_manager()
	{
		for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(g_file_manager->get_project_folder("./scripts/lua").get_path()))
		{
			if (dir_entry.is_regular_file())
			{
				const auto& file_path = dir_entry.path();
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
}