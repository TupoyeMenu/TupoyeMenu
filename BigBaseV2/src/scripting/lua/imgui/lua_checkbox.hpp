#pragma once
#include <imgui.h>
#include "lua_component.hpp"
#include "scripting/lua/lua_bindings.hpp"
#include "fiber_pool.hpp"

namespace big::lua::imgui
{
	class lua_checkbox : public lua_component
	{
		const char* m_text;
		bool m_value;

	public:
		lua_checkbox(const char* text, bool value) :
			lua_component(),
			m_text(text),
			m_value(value)
		{
		}

		void set_text(const char* text)
		{
			m_text = text;
		}

		bool is_enabled()
		{
			return m_value;
		}

		void set_enabled(bool val)
		{
			m_value = val;
		}

		void draw()
		{
			ImGui::Checkbox(m_text, &m_value);
		}
	};
}