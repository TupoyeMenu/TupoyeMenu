#include "backend/player_command.hpp"
#include "natives.hpp"
#include "util/ped.hpp"
#include "util/misc.hpp"

namespace big
{
	class slodpedcrash : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::AGGRESSIVE;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
			Ped ped = ped::spawn(ePedType::PED_TYPE_PROSTITUTE, RAGE_JOAAT("slod_human"), 0, misc::fvector3_to_Vector3(*player->get_ped()->get_position()), 0);
			script::get_current()->yield(3s);
			entity::delete_entity_notp(ped);
		}
	};

	slodpedcrash g_slodpedcrash("slodpedcrash", "Slod Ped Crash", "Spawns 'slod_human' ped near player wich crashes them.\nBlocked by most internal menus.\nUntested.\nThis crash is very old and unlikely to work.\nWill also crash you and other players if spawned too close.", 0);
}