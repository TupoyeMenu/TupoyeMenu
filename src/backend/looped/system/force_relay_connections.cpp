#include "backend/looped_command.hpp"
#include "pointers.hpp"

namespace big
{
	class force_relay_connections : looped_command
	{
		using looped_command::looped_command;

		virtual void on_tick() override
		{
			*g_pointers->m_gta.m_force_relay_connections = true;
		}

		virtual void on_disable() override
		{
			*g_pointers->m_gta.m_force_relay_connections = false;
		}
	};

	force_relay_connections g_force_relay_connections("forcerelays", "Force Relay Connections", "Force Relay Connections will hide your IP address from other players. This will also prevent you from seeing other people's actual IP address. Enabling this feature may cause you or other players to appear laggy and will cause issues with missions.",
	    g.protections.force_relay_connections);
}
