#pragma once
#include <imgui.h>
#include "lua_component.hpp"
#include "scripting/lua/lua_bindings.hpp"
#include "fiber_pool.hpp"

namespace big::lua::imgui
{
	class lua_separator : public lua_component
	{
	public:
		lua_separator() :
			lua_component()
		{
		}

		void draw()
		{
			ImGui::Separator();
		}
	};
}