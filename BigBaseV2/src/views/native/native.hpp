#pragma once

namespace big {
	class native_gui
	{
	public:
		static void draw_texture(std::string texturedic, std::string textureName, float x, float y, float size_x, float size_y, float heading, int r, int g, int b, int a);

		
	};
	inline native_gui g_native_gui;
}
