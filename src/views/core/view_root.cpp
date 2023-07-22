/**
 * @file view_root.cpp
 * @brief Draws ImGui windows and menubar.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

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

		if (g.window.about)
			view::about_view();

		view::menu_bar();
	}
}
