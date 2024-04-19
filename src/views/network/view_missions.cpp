/**
 * @file view_missions.cpp
 * @brief Starting freemode session events.
 */

#include "core/scr_globals.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "script_local.hpp"
#include "util/scripts.hpp"
#include "views/network/missions/cp_collection.hpp"
#include "views/network/missions/criminal_damage.hpp"
#include "views/network/missions/hunt_the_beast.hpp"
#include "views/network/missions/king_of_the_castle.hpp"
#include "views/view.hpp"

namespace big
{
	static bool mission_found = false;

	inline rage::scrThread* check_script(rage::joaat_t hash)
	{
		if (auto thread = gta_util::find_script_thread(hash))
		{
			mission_found = true;
			return thread;
		}

		return nullptr;
	}

	void view::missions()
	{
		mission_found = false;

		ImGui::TextUnformatted("Event Starter");

		ImGui::BeginGroup();
		components::button("Hot Target", [] {
			scripts::start_launcher_script(36);
		});
		components::button("Kill List", [] {
			scripts::start_launcher_script(37);
		});
		components::button("Checkpoints", [] {
			scripts::start_launcher_script(39);
		});
		components::button("Challenges", [] {
			scripts::start_launcher_script(40);
		});
		components::button("Penned In", [] {
			scripts::start_launcher_script(41);
		});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("Hot Property", [] {
			scripts::start_launcher_script(43);
		});
		components::button("King Of The Castle", [] {
			scripts::start_launcher_script(45);
		});
		components::button("Criminal Damage", [] {
			scripts::start_launcher_script(46);
		});
		components::button("Hunt The Beast", [] {
			scripts::start_launcher_script(47);
		});
		components::button("Business Battles", [] {
			scripts::start_launcher_script(114);
		});
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("One-On-One Deathmatch", [] {
			scripts::start_launcher_script(216);
		});
		components::button("Impromptu Race", [] {
			scripts::start_launcher_script(16);
		});
		components::button("Flight School", [] {
			scripts::start_launcher_script(215);
		});
		components::button("Golf", [] {
			scripts::start_launcher_script(212);
		});
		components::button("Tutorial", [] {
			scripts::start_launcher_script(20);
		});
		ImGui::SameLine(); components::help_marker("Only works on joining players");
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		components::button("Gunslinger", [] {
			scripts::start_launcher_script(230);
		});
		components::button("Space Monkey", [] {
			scripts::start_launcher_script(235);
		});
		components::button("Wizard", [] {
			scripts::start_launcher_script(231);
		});
		components::button("QUB3D", [] {
			scripts::start_launcher_script(236);
		});
		components::button("CAMHEDZ", [] {
			scripts::start_launcher_script(237);
		});
		ImGui::EndGroup();

		ImGui::SeparatorText("Current Event");

		if (check_script("am_criminal_damage"_J))
			render_criminal_damage_ui();

		if (check_script("am_cp_collection"_J))
			render_cp_collection_ui();

		if (check_script("am_king_of_the_castle"_J))
			render_king_of_the_castle_ui();

		if (check_script("am_hunt_the_beast"_J))
			render_hunt_the_beast_ui();

		if (!mission_found)
		{
			ImGui::TextUnformatted("No active mission");
		}

		ImGui::SeparatorText("MiniGames");

		components::command_button<"tutorialall">();
		ImGui::SameLine();
		components::command_button<"golfall">();
		ImGui::SameLine();
		components::command_button<"flightschoolall">();
		ImGui::SameLine();
		components::command_button<"dartsall">();

		components::command_button<"badlandsall">();
		ImGui::SameLine();
		components::command_button<"spacemonkeyall">();
		ImGui::SameLine();
		components::command_button<"wizardall">();

		components::command_button<"qub3dall">();
		ImGui::SameLine();
		components::command_button<"camhedzall">();
	}
}
