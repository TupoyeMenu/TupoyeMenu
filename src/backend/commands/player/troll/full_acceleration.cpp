#include "backend/player_command.hpp"
#include "natives.hpp"
#include "util/toxic.hpp"

namespace big
{
	class full_acceleration : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
			toxic::full_acceleration(player);
		}
	};

	full_acceleration g_full_acceleration("fullacceleration", "Full Acceleration", "Accelerates players vehicle to 3 times max speed. \nRequires entity control. \nBlocked by most internal menus.", 0);
}