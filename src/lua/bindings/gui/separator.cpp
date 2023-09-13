#if defined(ENABLE_LUA)

#include "separator.hpp"

namespace lua::gui
{
	void separator::draw()
	{
		ImGui::Separator();
	}
}

#endif // ENABLE_LUA
