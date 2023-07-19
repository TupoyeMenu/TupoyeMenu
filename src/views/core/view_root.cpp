#include "gui.hpp"
#include "views/view.hpp"
#include "lua/lua_manager.hpp"

namespace big
{
	void view::root()
	{
		if (g.window.main)
			view::main_view();

		if (g.window.player)
			view::view_player_tabs();

		if (g.window.users)
			view::players();

		if (g.window.demo)
			ImGui::ShowDemoWindow(&g.window.demo);

		if (g.window.chat)
			view::chat_tabs();

		view::menu_bar();
	}
}
