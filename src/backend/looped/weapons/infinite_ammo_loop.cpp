#include "backend/looped_command.hpp"
#include "natives.hpp"

namespace big
{
	class infinite_ammo_looped : looped_command
	{
		using looped_command::looped_command;

		virtual void on_tick() override
		{
			if (g_local_player == nullptr || g_local_player->m_weapon_manager == nullptr
			    || g_local_player->m_weapon_manager->m_weapon_info == nullptr
			    || g_local_player->m_weapon_manager->m_weapon_info->m_ammo_info == nullptr)
			{
				return;
			}

			Hash current_weapon = g_local_player->m_weapon_manager->m_selected_weapon_hash;
			int max_ammo        = g_local_player->m_weapon_manager->m_weapon_info->m_ammo_info->m_ammo_max_100;

			WEAPON::SET_PED_AMMO(self::ped, current_weapon, max_ammo, false);
		}

		virtual void on_disable() override
		{
			WEAPON::SET_PED_INFINITE_AMMO(self::ped, FALSE, NULL);
		}
	};

	infinite_ammo_looped
	    g_infinite_ammo_looped("infammoloop", "Infinite Ammo Looped", "Never run out of ammo again looped", g.weapons.infinite_ammo_loop);
}
