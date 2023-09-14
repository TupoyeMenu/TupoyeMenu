#if defined(ENABLE_LUA)

#include "sameline.hpp"

namespace lua::gui
{
	void sameline::draw()
	{
		ImGui::SameLine();
	}
}

#endif // ENABLE_LUA
