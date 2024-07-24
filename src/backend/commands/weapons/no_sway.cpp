#include "backend/bool_command.hpp"
#include "no_sway.hpp"

namespace big
{
	class no_sway : bool_command
	{
		using bool_command::bool_command;

		virtual void on_enable() override
		{
			weapons::m_no_sway_patch->apply();
		}

		virtual void on_disable() override
		{
			weapons::m_no_sway_patch->restore();
		}
	};
	no_sway g_no_sway("nosway", "No Sway", "Removes swaying from the sniper's scope when scoped in.", g.weapons.no_sway);
}