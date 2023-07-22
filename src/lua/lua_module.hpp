#pragma once
#include "bindings/gui/gui_element.hpp"
#include "lua_patch.hpp"
#include "sol.hpp"
#include "core/data/menu_event.hpp"

namespace big
{
	class script;

	class lua_module
	{
		std::unique_ptr<sol::state> m_state;

		std::string m_module_name;
		rage::joaat_t m_module_id;

		std::chrono::time_point<std::chrono::file_clock> m_last_write_time;

	public:
		std::vector<script*> m_registered_scripts;
		std::vector<std::shared_ptr<lua_patch>> m_registered_patches;

		std::vector<std::shared_ptr<lua::gui::gui_element>> m_independent_gui;
		std::unordered_map<rage::joaat_t, std::vector<std::shared_ptr<lua::gui::gui_element>>> m_gui;
		std::unordered_map<menu_event, std::vector<sol::protected_function>> m_event_callbacks;
		std::vector<void*> m_allocated_memory;

		lua_module(std::string module_name, folder& scripts_folder);
		~lua_module();

		rage::joaat_t module_id() const;
		const std::string& module_name() const;
		const std::chrono::time_point<std::chrono::file_clock> last_write_time() const;

		// used for sandboxing and limiting to only our custom search path for the lua require function
		void set_folder_for_lua_require(folder& scripts_folder);

		void sandbox_lua_os_library();
		void sandbox_lua_loads(folder& scripts_folder);

		void init_lua_api(folder& scripts_folder);
	};
}