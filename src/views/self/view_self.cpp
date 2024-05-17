#include "core/data/hud_component_names.hpp"
#include "fiber_pool.hpp"
#include "imgui.h"
#include "util/entity.hpp"
#include "util/scripts.hpp"
#include "views/view.hpp"

#include <script/globals/GPBD_FM_3.hpp>

namespace big
{
	extern bool user_updated_wanted_level;

	void view::self()
	{
		components::command_button<"suicide">();
		ImGui::SameLine();

		components::command_button<"heal">();
		ImGui::SameLine();
		components::command_button<"fillsnacks">();

		ImGui::SameLine();

		components::command_button<"clean">();
		ImGui::SameLine();
		components::command_button<"fillammo">();

		ImGui::SeparatorText("General");

		ImGui::BeginGroup();

		components::command_checkbox<"godmode">();
		components::command_checkbox<"otr">();
		const auto gpbd_fm_3 = scr_globals::gpbd_fm_3.as<GPBD_FM_3*>();
		if (g.self.off_radar && *g_pointers->m_gta.m_is_session_started && gpbd_fm_3->Entries[self::id].BossGoon.Boss == self::id)
			components::command_checkbox<"ghostorg">();
		ImGui::Checkbox("Phone Anim", &g.tunables.phone_anim);
		components::command_checkbox<"nophone">();
		components::command_checkbox<"mobileradio">();
		ImGui::Checkbox("Disable Help Text", &g.tunables.disable_help_text);
		components::command_checkbox<"fastrespawn">();
		ImGui::Checkbox("Dance Mode", &g.self.dance_mode);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		components::command_checkbox<"noclip">();
		components::options_modal("Noclip", [] { // TODO: This looks horrible, move or remove.
			ImGui::Separator();

			ImGui::BeginGroup();
			ImGui::TextUnformatted("No Clip Movement Speed Multiplier While Aiming");
			ImGui::SliderFloat("##noclipaimspeedmult", &g.self.noclip_aim_speed_multiplier, 0.1f, 1.0f);
			ImGui::TextUnformatted("No Clip Movement Speed Multiplier");
			ImGui::SliderFloat("##noclipspeedmult", &g.self.noclip_speed_multiplier, 1.f, 100.f);
			ImGui::EndGroup();
		});
		components::command_checkbox<"freecam">();
		components::command_checkbox<"noragdoll">();
		components::command_checkbox<"fastrun">();
		ImGui::Checkbox("Jump Ragdoll", &g.self.allow_ragdoll);
		components::command_checkbox<"noidlekick">();

		// clang-format off
		ImGui::BeginDisabled(!*g_pointers->m_gta.m_is_session_started ||
			gpbd_fm_3->Entries[self::id].BossGoon.Boss != -1 ||
			gta_util::find_script_thread("fm_mission_controller"_J) ||
			gta_util::find_script_thread("fm_mission_controller_2020"_J));
		// clang-format on
		components::command_checkbox<"passive">();
		ImGui::EndDisabled();

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		components::command_checkbox<"cleanloop">();
		components::command_checkbox<"nocollision">();
		components::command_checkbox<"gracefullanding">();
		components::command_checkbox<"infoxy">();    //
		components::command_checkbox<"walkunder">(); // WTF Who namend this??
		components::command_checkbox<"invis">();
		if (g.self.invisibility)
			components::command_checkbox<"localvis">(); // TODO: does nothing in SP
		components::command_checkbox<"interactionmenufreedom">();

		ImGui::EndGroup();

		ImGui::SeparatorText("Proofs");

		if (ImGui::Button("Check all"))
		{
			g.self.proof_bullet    = true;
			g.self.proof_fire      = true;
			g.self.proof_collision = true;
			g.self.proof_melee     = true;
			g.self.proof_explosion = true;
			g.self.proof_steam     = true;
			g.self.proof_water     = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Uncheck all"))
		{
			g.self.proof_bullet    = false;
			g.self.proof_fire      = false;
			g.self.proof_collision = false;
			g.self.proof_melee     = false;
			g.self.proof_explosion = false;
			g.self.proof_steam     = false;
			g.self.proof_water     = false;
		}

		ImGui::BeginGroup();

		ImGui::Checkbox("Bullet", &g.self.proof_bullet);
		ImGui::Checkbox("Fire", &g.self.proof_fire);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Collision", &g.self.proof_collision);
		ImGui::Checkbox("Melee", &g.self.proof_melee);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Explosion", &g.self.proof_explosion);
		ImGui::Checkbox("Steam", &g.self.proof_steam);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Water", &g.self.proof_water);

		ImGui::EndGroup();

		ImGui::SeparatorText("Wanted Level");

		ImGui::Checkbox("Never Wanted", &g.self.never_wanted);
		ImGui::SameLine();
		components::help_marker("You will never gain any wanted stars");

		// Only show all the other stuff like clear wanted, force wanted, and the slider if we don't have never_wanted enabled, since never_wanted overrides all of that
		if (!g.self.never_wanted)
		{
			ImGui::SameLine();
			components::command_button<"clearwantedself">();

			// Most ImGui widgets return true when they've been changed, so this is useful to prevent us from overwriting the wanted level's natural decay/progression if we're not keeping it locked
			ImGui::SetNextItemWidth(200);
			user_updated_wanted_level = ImGui::SliderInt("###wanted_level", &g.self.wanted_level, 0, 5);
			ImGui::SameLine();
			components::help_marker("Sets current wanted level; use with Force Wanted Level to lock it at the selected value");
			ImGui::SameLine();
			ImGui::Checkbox("Force Wanted Level", &g.self.force_wanted_level);
			ImGui::SameLine();
			components::help_marker("Keep a specific wanted level active on yourself");
		}

		ImGui::SeparatorText("HUD");

		ImGui::BeginGroup();

		ImGui::Checkbox("Hide Radar", &g.self.hud.hide_radar);

		ImGui::SameLine();

		ImGui::Checkbox("Hide Ammo", &g.self.hud.hide_ammo);

		ImGui::SameLine();

		ImGui::Checkbox("Force show HUD", &g.self.hud.force_show_hud);

		ImGui::Combo("##hud_comp_combo", &g.self.hud.selected_hud_component, hud_component_names, (int)HudComponents::HUD_WEAPONS);
		ImGui::SameLine();
		components::button("Hide", [] {
			g.self.hud.hud_components_states[g.self.hud.selected_hud_component] = true;
		});
		ImGui::SameLine();
		components::button("Show", [] {
			g.self.hud.hud_components_states[g.self.hud.selected_hud_component] = false;
		});

		components::button("Hide all", [] {
			g.self.hud.hide_radar = true;
			g.self.hud.hide_ammo  = true;

			for (int i = 0; i < (int)HudComponents::HUD_WEAPONS; i++)
			{
				g.self.hud.hud_components_states[i] = true;
			}
		});
		ImGui::SameLine();
		components::button("Show all", [] {
			g.self.hud.hide_radar = false;
			g.self.hud.hide_ammo  = false;

			for (int i = 0; i < (int)HudComponents::HUD_WEAPONS; i++)
			{
				g.self.hud.hud_components_states[i] = false;
			}
		});
		ImGui::SameLine();
		ImGui::Checkbox("Force show HUD element", &g.self.hud.force_show_hud_element);
		ImGui::SameLine();
		components::help_marker("To force show a HUD specific element, click Hide all then click Show on the desired element.");

		ImGui::EndGroup();

		g.self.proof_mask = 0;
		if (g.self.god_mode)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::GOD);
		}
		if (g.self.proof_bullet)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::BULLET);
		}
		if (g.self.proof_fire)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::FIRE);
		}
		if (g.self.proof_collision)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::COLLISION);
		}
		if (g.self.proof_melee)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::MELEE);
		}
		if (g.self.proof_explosion)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::EXPLOSION);
		}
		if (g.self.proof_steam)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::STEAM);
		}
		if (g.self.proof_water)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::WATER);
		}
	}
}
