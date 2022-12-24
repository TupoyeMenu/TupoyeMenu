#include "backend/player_command.hpp"
#include "natives.hpp"
#include "util/toxic.hpp"

namespace big
{
	class launch_vehicle_up : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
			toxic::launch_vehicle_up(player);
		}
	};

	launch_vehicle_up g_launch_vehicle_up("launchvehicleup", "Launch Vehicle Up", "Launches players vehicle up. \nRequires entity control. \nBlacked by most internal menus.\nUses max sync speed on other players, uses INT32_MAX on local player.", 0);
}