#pragma once
#include "lua_bindings.hpp"
#include "lua_script.hpp"

namespace big
{
	class lua_manager
	{
		std::vector<std::unique_ptr<lua_script>> m_scripts;

	public:
		lua_manager();
		~lua_manager();

		void draw_script_gui(std::uint32_t menu);
	};

	inline lua_manager* g_lua_manager{};
}