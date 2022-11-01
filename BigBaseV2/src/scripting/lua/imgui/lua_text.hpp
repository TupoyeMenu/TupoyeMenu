#pragma once
#include <imgui.h>
#include "lua_component.hpp"
#include "scripting/lua/lua_bindings.hpp"
#include "fiber_pool.hpp"
#include "gta/joaat.hpp"

namespace big::lua::imgui
{
	class lua_text : public lua_component
	{
		const char* m_text;
		ImFont* m_font = nullptr;

	public:
		lua_text(const char* text) :
			lua_component(),
			m_text(text)
		{
		}

		void set_text(const char* text)
		{
			m_text = text;
		}

		void set_font(const char* font)
		{
			auto hash = rage::joaat(font);

			switch (hash)
			{
				case RAGE_JOAAT("title"):
					m_font = g->window.font_title;
					break;
				case RAGE_JOAAT("subtitle"):
					m_font = g->window.font_sub_title;
					break;
				case RAGE_JOAAT("small"):
					m_font = g->window.font_small;
					break;
				case RAGE_JOAAT("icon"):
					m_font = g->window.font_icon;
					break;
				default:
					m_font = nullptr;
					break;
			}
		}

		void draw()
		{
			if (m_font)
				ImGui::PushFont(m_font);
			ImGui::TextWrapped("%s", m_text);
			if (m_font)
				ImGui::PopFont();
		}
	};
}