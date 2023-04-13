#include "views/view.hpp"

namespace big
{
	void view::protection_settings()
	{
		ImGui::BeginGroup();
		ImGui::Checkbox("Bounty", &g.protections.script_events.bounty);
		ImGui::Checkbox("CEO Money", &g.protections.script_events.ceo_money);
		ImGui::Checkbox("Fake Deposit", &g.protections.script_events.fake_deposit);
		ImGui::Checkbox("Force Mission", &g.protections.script_events.force_mission);
		ImGui::Checkbox("Force Teleport", &g.protections.script_events.force_teleport);
		ImGui::Checkbox("GTA Banner", &g.protections.script_events.gta_banner);
		ImGui::Checkbox("MC Teleport", &g.protections.script_events.mc_teleport);
		ImGui::Checkbox("Block RID Joining", &g.protections.rid_join);
		ImGui::SameLine(); components::help_marker("This will block anyone trying to join, kick or crash you with your Rockstar ID, including your friends");
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
		ImGui::Checkbox("Lessen Breakup Kicks As Host", &g.protections.lessen_breakups);
		ImGui::SameLine(); components::help_marker("Attacker must join after you have become host for this to work. There are anti-cheat concerns with this feature");
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Start Activity", &g.protections.script_events.start_activity);
		ImGui::Checkbox("Send SMS", &g.protections.script_events.send_sms);
		ImGui::Checkbox("Spectate", &g.protections.script_events.spectate);
		ImGui::Checkbox("Vehicle Kick", &g.protections.script_events.vehicle_kick);
		ImGui::Checkbox("Wanted Level", &g.protections.script_events.clear_wanted_level);
		ImGui::Checkbox("Desync Kick", &g.protections.desync_kick);
		ImGui::Checkbox("Request Controll", &g.protections.request_control);
		ImGui::Checkbox("Receive Pickup", &g.protections.receive_pickup);
		ImGui::SameLine(); components::help_marker("This prevents any pickup from the ground such as unwanted money drops.\nAttention: Normal pickups are also no longer possible.");
		ImGui::Checkbox("Admin Check", &g.protections.admin_check);
		ImGui::EndGroup();
	}

}
