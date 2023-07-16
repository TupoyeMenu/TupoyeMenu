/**
 * @file view_protection_settings.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "views/view.hpp"

namespace big
{
	static inline void set_all_protections(bool state)
	{
		for (size_t i = (size_t)&g.protections; i <= (size_t) & (g.protections.kick_rejoin); i++)
			*(bool*)i = state;
	}

	void view::protection_settings()
	{
		auto initial_protections = g.protections;

		ImGui::BeginGroup();
		ImGui::Checkbox("Bounty", &g.protections.script_events.bounty);
		ImGui::Checkbox("CEO Money", &g.protections.script_events.ceo_money);
		ImGui::Checkbox("Fake Deposit", &g.protections.script_events.fake_deposit);
		ImGui::Checkbox("Force Mission", &g.protections.script_events.force_mission);
		ImGui::Checkbox("Force Teleport", &g.protections.script_events.force_teleport);
		ImGui::Checkbox("GTA Banner", &g.protections.script_events.gta_banner);
		ImGui::Checkbox("MC Teleport", &g.protections.script_events.mc_teleport);
#ifndef ENABLE_SOCIALCLUB
		ImGui::BeginDisabled(true);
#endif // ENABLE_SOCIALCLUB
		ImGui::Checkbox("Block RID Joining", &g.protections.rid_join);
		ImGui::SameLine(); components::help_marker("This will block anyone trying to join, kick or crash you with your Rockstar ID, including your friends");
#ifndef ENABLE_SOCIALCLUB
		ImGui::EndDisabled();
		ImGui::SameLine(); components::help_marker("This feature is currently disabled, Enable `ENABLE_SOCIALCLUB` to enable.");
#endif // ENABLE_SOCIALCLUB
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Send to Cutscene", &g.protections.script_events.send_to_cutscene);
		ImGui::Checkbox("Send to Location", &g.protections.script_events.send_to_location);
		ImGui::Checkbox("Sound Spam", &g.protections.script_events.sound_spam);
		ImGui::Checkbox("Personal Vehicle Destroyed", &g.protections.script_events.personal_vehicle_destroyed);
		ImGui::Checkbox("Remote Off Radar", &g.protections.script_events.remote_off_radar);
		ImGui::Checkbox("Rotate Cam", &g.protections.script_events.rotate_cam);
		ImGui::Checkbox("Teleport To Warehouse", &g.protections.script_events.teleport_to_warehouse);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Start Activity", &g.protections.script_events.start_activity);
		ImGui::Checkbox("Send SMS", &g.protections.script_events.send_sms);
		ImGui::Checkbox("Spectate", &g.protections.script_events.spectate);
		ImGui::Checkbox("Vehicle Kick", &g.protections.script_events.vehicle_kick);
		ImGui::Checkbox("Wanted Level", &g.protections.script_events.clear_wanted_level);
		ImGui::Checkbox("Request Controll", &g.protections.request_control);
		ImGui::Checkbox("Receive Pickup", &g.protections.receive_pickup);
		ImGui::SameLine(); components::help_marker("This prevents any pickup from the ground such as unwanted money drops.\nAttention: Normal pickups are also no longer possible.");
		ImGui::Checkbox("Admin Check", &g.protections.admin_check);
		ImGui::Checkbox("Kick Rejoin", &g.protections.kick_rejoin);
		ImGui::EndGroup();

		ImGui::SeparatorText("Options");
		ImGui::BeginGroup();
		if (ImGui::Button("Enable All Protections"))
			set_all_protections(true);
		ImGui::SameLine();
		if (ImGui::Button("Disable All Protections"))
			set_all_protections(false);
		ImGui::SameLine();
		if (ImGui::Button("Reset Protections"))
			g.protections = initial_protections;
		ImGui::EndGroup();
	};
}
