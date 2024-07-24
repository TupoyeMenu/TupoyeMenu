#include "backend/looped_command.hpp"
#include "util/ped.hpp"

namespace big
{
	class clean_player_looped : looped_command
	{
		using looped_command::looped_command;

		virtual void on_tick() override
		{
			ped::clean_ped(self::ped);
		}
	};

	clean_player_looped g_clean_player_looped("cleanloop", "Keep Player Clean", "Prevents wetness and decals from being applied on you", g.self.clean_player);
}
