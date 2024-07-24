#include "backend/player_command.hpp"
#include "natives.hpp"
#include "util/vehicle.hpp"

namespace big
{
	class upgrade_vehicle : player_command
	{
		using player_command::player_command;

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			Ped ped         = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);

			if (!PED::IS_PED_IN_ANY_VEHICLE(ped, true))
			{
				g_notification_service.push_warning("Toxic", "Target player is not in a vehicle.");
			}
			else
			{
				vehicle::max_vehicle(vehicle);
			}
		}
	};

	upgrade_vehicle g_upgrade_vehicle("upgradeveh", "Max Vehicle Mods", "Sets all vehicle mods to their max value.", 0);
}
