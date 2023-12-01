/**
 * @file view_hotkey_settings.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "imgui.h"
#include "services/hotkey/hotkey_service.hpp"
#include "views/view.hpp"
#include "widgets/imgui_hotkey.hpp"

namespace big
{
	void view::hotkey_settings()
	{
		ImGui::PushItemWidth(350.f);

		ImGui::SeparatorText("System");
		if (ImGui::Hotkey("Menu Toggle", &g.settings.hotkeys.menu_toggle))
			g.settings.hotkeys.editing_menu_toggle = true; // make our menu reappear
		if (ImGui::Hotkey("Toggle Command Executor", &g.settings.hotkeys.cmd_excecutor))
			g_hotkey_service->update_hotkey("cmdexecutor", g.settings.hotkeys.cmd_excecutor);
		if (ImGui::Hotkey("Rage Quit (Like Alt + F4)", &g.settings.hotkeys.fast_quit))
			g_hotkey_service->update_hotkey("quit", g.settings.hotkeys.fast_quit);

		ImGui::SeparatorText("Teleport");
		if (ImGui::Hotkey("Teleport to waypoint", &g.settings.hotkeys.teleport_waypoint))
			g_hotkey_service->update_hotkey("waypoint", g.settings.hotkeys.teleport_waypoint);
		if (ImGui::Hotkey("Teleport to Objective", &g.settings.hotkeys.teleport_objective))
			g_hotkey_service->update_hotkey("objective", g.settings.hotkeys.teleport_objective);
		if (ImGui::Hotkey("Teleport to Selected Blip", &g.settings.hotkeys.teleport_selected))
			g_hotkey_service->update_hotkey("highlighttp", g.settings.hotkeys.teleport_selected);
		if (ImGui::Hotkey("Teleport into Personal Vehicle", &g.settings.hotkeys.teleport_pv))
			g_hotkey_service->update_hotkey("pvtp", g.settings.hotkeys.teleport_pv);
		if (ImGui::Hotkey("Bring PV", &g.settings.hotkeys.bringvehicle))
			g_hotkey_service->update_hotkey("bringpv", g.settings.hotkeys.bringvehicle);

		ImGui::SeparatorText("Movement");
		if (ImGui::Hotkey("Toggle Noclip", &g.settings.hotkeys.noclip))
			g_hotkey_service->update_hotkey("noclip", g.settings.hotkeys.noclip);
		if (ImGui::Hotkey("Toggle fastrun", &g.settings.hotkeys.superrun))
			g_hotkey_service->update_hotkey("fastrun", g.settings.hotkeys.superrun);
		if (ImGui::Hotkey("Toggle Freecam", &g.settings.hotkeys.freecam))
			g_hotkey_service->update_hotkey("freecam", g.settings.hotkeys.freecam);


		ImGui::SeparatorText("Inventory");
		if (ImGui::Hotkey("Heal", &g.settings.hotkeys.heal))
			g_hotkey_service->update_hotkey("heal", g.settings.hotkeys.heal);
		if (ImGui::Hotkey("Fill Snacks", &g.settings.hotkeys.fill_inventory))
			g_hotkey_service->update_hotkey("fillsnacks", g.settings.hotkeys.fill_inventory);
		if (ImGui::Hotkey("Fill Ammo", &g.settings.hotkeys.fill_ammo))
			;
		g_hotkey_service->update_hotkey("fillammo", g.settings.hotkeys.fill_ammo);

		ImGui::SeparatorText("Visibility");
		if (ImGui::Hotkey("Toggle invisibility", &g.settings.hotkeys.invis))
			g_hotkey_service->update_hotkey("invis", g.settings.hotkeys.invis);
		if (ImGui::Hotkey("Toggle Vehicle Invisibility", &g.settings.hotkeys.invisveh))
			g_hotkey_service->update_hotkey("invisveh", g.settings.hotkeys.invisveh);
		if (ImGui::Hotkey("Toggle Local Vehicle Invisibility", &g.settings.hotkeys.localinvisveh))
			g_hotkey_service->update_hotkey("localinvisveh", g.settings.hotkeys.localinvisveh);

		ImGui::SeparatorText("Misc");
		if (ImGui::Hotkey("Skip Cutscene", &g.settings.hotkeys.skip_cutscene))
			g_hotkey_service->update_hotkey("skipcutscene", g.settings.hotkeys.skip_cutscene);
		if (ImGui::Hotkey("Clear Wanted", &g.settings.hotkeys.clear_wanted))
			g_hotkey_service->update_hotkey("clearwantedlvl", g.settings.hotkeys.clear_wanted);
		if (ImGui::Hotkey("Toggle passive mode", &g.settings.hotkeys.passive))
			g_hotkey_service->update_hotkey("passive", g.settings.hotkeys.passive);

		ImGui::PopItemWidth();
	}
}
