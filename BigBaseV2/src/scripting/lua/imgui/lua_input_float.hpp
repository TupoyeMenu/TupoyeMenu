#pragma once
#include <imgui.h>
#include "lua_component.hpp"
#include "scripting/lua/lua_bindings.hpp"
#include "fiber_pool.hpp"

namespace big::lua::imgui
{
	class lua_input_float : public lua_component
	{
		const char* m_text;
		float m_value;

	public:
		lua_input_float(const char* text, float value) :
			lua_component(),
			m_text(text),
			m_value(value)
		{
		}

		void set_text(const char* text)
		{
			m_text = text;
		}

		float get_value()
		{
			return m_value;
		}

		void set_value(float val)
		{
			m_value = val;
		}

		void draw()
		{
			ImGui::InputFloat(m_text, &m_value);
		}
	};
}