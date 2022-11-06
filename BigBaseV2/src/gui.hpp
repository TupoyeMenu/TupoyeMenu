#pragma once
#include "common.hpp"

namespace big
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();
		void always_draw();

		void script_on_tick();
		static void script_func();
	public:
		const char* texture = "\\TupoyeMenu\\tupoye.ytd";
		bool m_opened{};
		bool m_start_topoye = true;
	};

	inline gui g_gui;
}
