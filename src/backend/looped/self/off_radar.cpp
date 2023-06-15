#include "backend/looped_command.hpp"
#include "core/scr_globals.hpp"
#include "natives.hpp"

#include <script/globals/GlobalPlayerBD.hpp>

namespace big
{
	class off_radar : looped_command
	{
		using looped_command::looped_command;

		virtual void on_tick() override
		{
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].OffRadarActive = true;
			*scr_globals::offradar_time.at(57).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;
		}

		virtual void on_disable() override
		{
			scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].OffRadarActive = false;
		}
	};

	off_radar g_off_radar("otr", "Off Radar", "Hides your blip from other players", g.self.off_radar);
}
