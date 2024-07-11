/**
 * @file view.hpp
 * @brief GUI window function declarations.
 */

#pragma once
#include "esp/view_esp.hpp"
#include "gui/components/components.hpp"

namespace big
{
	class view
	{
	public:
		static void about_view();
		static void main_view();

		static void debug_animations(std::string* dict = nullptr, std::string* anim = nullptr); // Can be used to retrieve animations
		static void debug_script_events();
		static void debug_globals();
		static void debug_locals();
		static void debug_misc();
		static void debug_threads();
		static void debug_tabs();

		static void esp_settings();
		static void outfit_editor();
		static void outfit_slots();
		static void stat_editor();
		static void gui_settings();
		static void hotkey_settings();
		static void reaction_settings();
		static void protection_settings();
		static void context_menu_settings();
		static void mobile();
		static void notifications();
		static void overlay();
		static void root();
		static void self_tabs();
		static void self();
		static void animations();
		static void network();
		static void settings_tabs();
		static void network_controls();
		static void chat();
		static void missions();
		static void player_database();
		static void session_browser();
		static void settings();
		static void vehicle_tabs();
		static void vehicle();
		static void lsc();
		static void spawn_vehicle();
		static void pv();
		static void xml_vehicles();
		static void fun_vehicle();
		static void spawn_ped();
		static void world_tabs();
		static void world();
		static void ambient();
		static void time_and_weather();
		static void network_tabs();
		static void gravity();
		static void ocean();
		static void waypoint_and_objective();
		static void spoofing();
		static void teleport();
		static void custom_teleport();
		static void players();
		static void weapons();
		static void context_menu();
		static void gta_data();
		static void menu_bar();
		static void creator();
		static void blackhole();
		static void model_swapper();
		static void xml_maps();
		static void gta_cache();
		static void lua_scripts();
		static void vfx();

		static void onboarding();

		static void cmd_executor();

		static void view_player_tabs();
		static void view_player_info();
		static void view_player_kicks_and_crashes();
		static void view_player_misc();
		static void view_player_toxic();
		static void view_player_troll();
		static void view_player_vehicle();
	};
}
