#include "backend/player_command.hpp"
#include "natives.hpp"
#include "util/ped.hpp"
#include "util/misc.hpp"

namespace big
{
	class spawn_griefer_jesus : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
            Ped player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
			Vector3 pos = misc::fvector3_to_Vector3(*player->get_ped()->get_position()) + Vector3(0, 0, 10);
			ped::spawn_griefer_jesus(pos, player_ped);
		}
	};

    class spawn_extrime_griefer_jesus : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
            Ped player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
			Vector3 pos = misc::fvector3_to_Vector3(*player->get_ped()->get_position()) + Vector3(0, 0, 10);
			ped::spawn_extrime_griefer_jesus(pos, player_ped);
		}
	};

    class spawn_griefer_lazer : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
            Ped player_ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
			Vector3 pos = misc::fvector3_to_Vector3(*player->get_ped()->get_position()) + Vector3(0, 0, 10);
			ped::spawn_griefer_jet(pos, player_ped, RAGE_JOAAT("lazer"));
		}
	};

	spawn_griefer_jesus g_spawn_griefer_jesus("spawn_grief_jesus", "Spawn Griefer Jesus", "Spawns 'Griefer Jesus' just like in ChaosMod", 0);
	spawn_extrime_griefer_jesus g_spawn_extrime_griefer_jesus("spawn_extr_grief_jesus", "Spawn Extreme Griefer Jesus", "Spawns 'Extreme Griefer Jesus' just like in ChaosMod", 0);
	spawn_griefer_lazer g_spawn_griefer_lazer("spawn_grief_lazer", "Spawn Griefer Lazer", "Spawns a lazer with Griefer Jesus in it", 0);
}