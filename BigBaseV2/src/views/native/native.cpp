#include "natives.hpp"
#include "native.hpp"

namespace big
{


	void native_gui::draw_texture(std::string texturedic, std::string texturename, float x, float y, float size_x, float size_y, float heading, int r, int g, int b, int a)
	{
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(texturedic.c_str(), false);
		if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(texturedic.c_str())) GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(texturedic.c_str(), false);
		GRAPHICS::DRAW_SPRITE(texturedic.c_str(), texturename.c_str(),  x,  y,  size_x,  size_y,  heading,  r,  g,  b,  a, 0,0);
	}
}