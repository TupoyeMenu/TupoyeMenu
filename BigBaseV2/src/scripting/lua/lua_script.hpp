#pragma once
#include "script.hpp"
#include "lua_bindings.hpp"
#include "imgui/lua_component.hpp"

namespace big
{
	namespace lua::script
	{
		void load(sol::state& state);
	}

	class lua_script
	{
	public:
		sol::state m_state;
		std::optional<int> m_script_tick_function;
		std::vector<script*> m_scripts;
		std::unordered_map<std::uint32_t, std::vector<std::shared_ptr<lua::imgui::lua_component>>> m_script_gui;

		lua_script(std::filesystem::path path);
		~lua_script();
	};
}