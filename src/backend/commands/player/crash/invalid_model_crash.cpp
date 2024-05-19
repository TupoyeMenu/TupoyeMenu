#ifdef ENABLE_CRASHES
#include "backend/player_command.hpp"
#include "natives.hpp"
#include "util/ped.hpp"
#include "util/misc.hpp"
#include "util/world_model.hpp"
#include "util/teleport.hpp"

namespace big
{
	class invalidmodelcrash : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level() override
		{
			return CommandAccessLevel::ADMIN;
		}

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			Vector3 original_pos = self::pos;
			Vector3 target_pos = misc::fvector3_to_Vector3(*player->get_ped()->get_position());
			teleport::to_coords(Vector3(10000, 10000, 50000));
			script::get_current()->yield(2s);
			Ped ped = ped::spawn(ePedType::PED_TYPE_PROSTITUTE, "slod_human"_J, 0, target_pos, 0);
			Object prop = world_model::spawn("proc_drygrasses01"_J, target_pos, true);
			Vehicle veh = vehicle::spawn("astron2"_J, target_pos, 0.f);

			script::get_current()->yield(4s);
			entity::delete_entity(ped);
			entity::delete_entity(prop);
			entity::delete_entity(veh);
			script::get_current()->yield(10s);
			teleport::to_coords(original_pos);
		}
	};

	invalidmodelcrash g_invalidmodelcrash("invalidmodelcrash", "Invalid Model Crash", "Spawns crash models near player wich crashes them.\nBlocked by most internal menus.\nWill also crash other players near the target!", 0);
}
#endif // ENABLE_CRASHES