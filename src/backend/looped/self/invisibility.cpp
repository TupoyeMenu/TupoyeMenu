#include "backend/looped_command.hpp"
#include "natives.hpp"
#include "services/script_patcher/script_patcher_service.hpp"

namespace big
{
	class invisibility : looped_command
	{
		using looped_command::looped_command;

		virtual void on_enable() override
		{
			g_script_patcher_service->update();
		}

		virtual void on_tick() override
		{
			ENTITY::SET_ENTITY_VISIBLE(self::ped, false, 0);

			if (g.self.local_visibility)
				NETWORK::SET_ENTITY_LOCALLY_VISIBLE(self::ped);
		}

		virtual void on_disable() override
		{
			ENTITY::SET_ENTITY_VISIBLE(self::ped, true, 0);
			g_script_patcher_service->update();
		}
	};

	invisibility g_invisibility("invis", "Invisibility", "Makes you invisible", g.self.invisibility);
	bool_command g_local_visibility("localvis", "Visible Locally", "Makes you visible to yourself, but other players would still not be able to see you", g.self.local_visibility);
}