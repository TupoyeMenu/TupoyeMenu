#pragma once
#include <imgui.h>
#include "lua_component.hpp"
#include "scripting/lua/lua_bindings.hpp"
#include "fiber_pool.hpp"

namespace big::lua::imgui
{
	class lua_button : public lua_component
	{
		const char* m_text;
		sol::function m_callback;

	public:
		lua_button(const char* text, sol::function callback) : 
			lua_component(),
			m_text(text),
			m_callback(callback)
		{
		}

		void set_text(const char* text)
		{
			m_text = text;
		}

		void draw()
		{
			if (ImGui::Button(m_text))
			{
				g_fiber_pool->queue_job([this]
				{
					m_callback();
				});
			}
		}
	};
}