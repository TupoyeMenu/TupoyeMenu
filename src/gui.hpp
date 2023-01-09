#pragma once
#include "common.hpp"

namespace big
{
	class gui
	{
	public:
		gui();
		virtual ~gui();
		gui(const gui&) = delete;
		gui(gui&&) noexcept  = delete;
		gui& operator=(const gui&) = delete;
		gui& operator=(gui&&) noexcept  = delete;

		bool is_open();
		bool is_mouse_active();
		void toggle(bool toggle, bool should_toggle_mouse = true);
		void toggle_mouse(bool toggle);

		void dx_init();
		void dx_on_tick();

		void script_on_tick();
		static void script_func();

		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		bool m_is_open;
		bool m_is_mouse_active = false;

	};

	inline gui* g_gui;
}
