#include "backend/player_command.hpp"
#include "core/scr_globals.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "util/scripts.hpp"

#include <script/globals/GPBD_FM_3.hpp>

namespace big
{
	template<rage::joaat_t script_hash>
	class start_script : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level() override
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			scripts::force_script_on_player(player, script_hash);
		}
	};

	start_script<"fm_intro"_J> g_start_tutorial("tutorial", "Start Tutorial", "Plays that unskippable cutscene used in the tutorial.", 0);
	start_script<"golf_mp"_J> g_start_golf("golf", "Start Golf", "Starts a game of golf.", 0);
	start_script<"Pilot_School_MP"_J> g_start_flight_school("flightschool", "Start Flight School", "Teleports player to the airport and starts flight school.", 0);
	start_script<"AM_Darts"_J> g_start_darts("darts", "Start Darts", "Starts a game of darts in the Sandy Shores bar.", 0);
	start_script<"gunslinger_arcade"_J> g_start_badlands("badlands", "Start Badlands Revenge II", "Starts the arcade game Badlands Revenge II in fullscreen.", 0);
	start_script<"ggsm_arcade"_J> g_start_space_monkey("spacemonkey", "Start Space Monkey 3", "Starts the arcade game Space Monkey 3 in fullscreen.", 0);
	start_script<"wizard_arcade"_J> g_start_wizards_ruin("wizard", "Start The Wizard's Ruin", "Starts the arcade game The Wizard's Ruin in fullscreen.", 0);
	start_script<"puzzle"_J> g_start_qub3d("qub3d", "Start QUB3D", "Starts the arcade game QUB3D in fullscreen.", 0);
	start_script<"camhedz_arcade"_J> g_start_camhedz("camhedz", "Start CAMHEDZ", "Starts the arcade game CAMHEDZ in fullscreen.", 0);
}