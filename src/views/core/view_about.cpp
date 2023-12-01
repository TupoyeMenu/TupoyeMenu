/**
 * @file view_about.cpp
 * @brief Credits and licensing information.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "views/view.hpp"

namespace big
{

const char* about_text = R"(# TupoyeMenu Made possible by
Menus - Bases we use:
* [YimMenu](https://github.com/YimMenu/YimMenu) TupoyeMenu is entirely based on YimMenu.
* [BigBaseV2-fix](https://bitbucket.org/gir489/bigbasev2-fix) Asi Loader, YimMenu is based on BigBaseV2.
* [BigBaseV2](https://github.com/Pocakking/BigBaseV2) YimMenu is based on BigBaseV2.

Libraries we use:
* [GTAV-Classes](https://github.com/Yimura/GTAV-Classes) GTA 5 class defenitions.
* [AsyncLogger](https://github.com/Yimura/AsyncLogger) The log library we use.
* [imgui_markdown](https://github.com/juliettef/imgui_markdown) markdown for Dear ImGui.
* [ImGui](https://github.com/ocornut/imgui) User inteface.
* [nlohmann json](https://github.com/nlohmann/json) Json parsing.
* [pugixml](https://github.com/zeux/pugixml) XML parsing.
* [MinHook](https://github.com/TsudaKageyu/minhook) Hooking functions.
# License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <[https://www.gnu.org/licenses/](https://www.gnu.org/licenses/)>.
# Donate
I don't recommend donating bacause i'm just a moron that has not done any important changes to this menu.
If you still want to do it here is my XMR address:
)";

	void view::about_view()
	{
		if (ImGui::Begin("About", &g.window.about))
		{
			static char xmr_address[96] = "8Bma7KYCG4xPWibfHNZ7sYHYjN8jzDV165qqUPfpbELVC9dTDZuJFHSd21RbGgB8vKHnHZWwQ2zmPfWzZ8JMujYzLGxn1vm";
            components::markdown(about_text);

			ImGui::TextUnformatted(xmr_address);

            if (ImGui::Button("Copy"))
				ImGui::SetClipboardText(xmr_address);
	    }
        ImGui::End();
    }
}
