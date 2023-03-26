#pragma once
#include "esp/view_esp.hpp"
#include "gui/components/components.hpp"
#include "util/animator.hpp"

namespace big
{
	class view
	{
		inline static animator window_animator      = animator();
		inline static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

	public:
		static void main_view();
		static void debug_script_events();
		static void debug_globals();
		static void debug_misc();
		static void debug_threads();
		static void debug_tabs();

		static void esp_settings();
		static void context_menu_settings();
		static void outfit_editor();
		static void outfit_slots();
		static void stat_editor();
		static void gui_settings();
		static void hotkey_settings();
		static void reaction_settings();
		static void protection_settings();
		static void heading();
		static void mobile();
		static void notifications();
		static void spinner();
		static void root();
		static void self_tabs();
		static void self();
		static void outfit_editor(Ped ped_to_edit);
		static void session();
		static void settings_tabs();
		static void missions();
		static void player_database();
		static void session_browser();
		static void settings();
		static void vehicle_tabs();
		static void vehicle();
		static void lsc();
		static void spawn_vehicle();
		static void pv();
		static void fun_vehicle();
		static void spawn_ped();
		static void world_tabs();
		static void ambient();
		static void time_and_weather();
		static void network_tabs();
		static void spoofing();
		static void teleport();
		static void players();
		static void weapons();
		static void context_menu();
		static void gta_data();
		static void menu_bar();
		static void overlay();
		static void chat_tabs();
		static void chat();
		static void chat_dm();
		static void creator();
		static void blackhole();
		static void model_swapper();
		static void nearby();

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
