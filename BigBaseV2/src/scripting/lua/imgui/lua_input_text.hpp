#pragma once
#include <imgui.h>
#include "lua_component.hpp"
#include "scripting/lua/lua_bindings.hpp"
#include "fiber_pool.hpp"

namespace big::lua::imgui
{
	class lua_input_text : public lua_component
	{
		const char* m_text;
		char m_buffer[0xFF];

	public:
		lua_input_text(const char* text, const char* value) :
			lua_component(),
			m_text(text)
		{
			strncpy(m_buffer, value, sizeof(m_buffer));
		}

		void set_text(const char* text)
		{
			m_text = text;
		}

		char* get_value()
		{
			return m_buffer;
		}

		void set_value(const char* val)
		{
			strncpy(m_buffer, val, sizeof(m_buffer));
		}

		void draw()
		{
			ImGui::InputText(m_text, m_buffer, sizeof(m_buffer));
		}
	};
}