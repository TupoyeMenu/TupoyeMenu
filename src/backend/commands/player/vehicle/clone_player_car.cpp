#include "backend/player_command.hpp"
#include "natives.hpp"
#include "services/vehicle/persist_car_service.hpp"
#include "util/entity.hpp"

namespace big
{
	class clone_player_car : player_command
	{
		using player_command::player_command;

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			Player player_id = player->id();
			Ped ped          = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player_id);
			if (!PED::IS_PED_IN_ANY_VEHICLE(ped, true))
			{
				g_notification_service.push_warning("Toxic", "Target player is not in a vehicle.");
			}
			else
			{
				Vehicle v = persist_car_service::clone_ped_car(ped, PED::GET_VEHICLE_PED_IS_USING(ped));
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), v, -1);
			}
		}
	};

	clone_player_car g_clone_player_car("cloneplayercar", "Spawn Clone", "", 0);
}
