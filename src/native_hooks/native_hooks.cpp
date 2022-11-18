#include "native_hooks.hpp"
#include "all_scripts.hpp"
#include "carmod_shop.hpp"
#include "freemode.hpp"
#include "maintransition.hpp"
#include "gta_util.hpp"
#include "shop_controller.hpp"

#include <script/scrProgram.hpp>
#include <script/scrProgramTable.hpp>

namespace big
{
    constexpr auto ALL_SCRIPT_HASH = RAGE_JOAAT("ALL_SCRIPTS");

    native_hooks::native_hooks()
    {
        add_native_detour(0x812595A0644CE1DE, all_scripts::IS_DLC_PRESENT);
        add_native_detour(0x5D10B3795F3FC886, all_scripts::NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA); // Speed up sesssion join. From https://github.com/YimMenu/YimMenu/discussions/143
        add_native_detour(0x95914459A87EBA28, all_scripts::NETWORK_BAIL);
        add_native_detour(0x6BFB12CE158E3DD4, all_scripts::SC_TRANSITION_NEWS_SHOW); // Stops news.
        add_native_detour(0xFE4C1D0D3B9CC17E, all_scripts::SC_TRANSITION_NEWS_SHOW_TIMED); // Stops news.
        add_native_detour(RAGE_JOAAT("carmod_shop"), 0x06843DA7060A026B, carmod_shop::SET_ENTITY_COORDS);
        add_native_detour(RAGE_JOAAT("carmod_shop"), 0x8E2530AA8ADA980E, carmod_shop::SET_ENTITY_HEADING);
        add_native_detour(RAGE_JOAAT("carmod_shop"), 0x34E710FF01247C5A, carmod_shop::SET_VEHICLE_LIGHTS);
        add_native_detour(RAGE_JOAAT("carmod_shop"), 0x767FBC2AC802EF3D, carmod_shop::STAT_GET_INT);
        add_native_detour(RAGE_JOAAT("maintransition"), 0x6F3D4ED9BEE4E61D, maintransition::NETWORK_SESSION_HOST); // RID Joiner from https://github.com/YimMenu/YimMenu/issues/172
        add_native_detour(RAGE_JOAAT("maintransition"), 0x933BBEEB8C61B5F4, maintransition::IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED); // This hook lets you stop player-switch in "Pre-HUD Checks"
        add_native_detour(RAGE_JOAAT("maintransition"), 0x06843DA7060A026B, maintransition::SET_ENTITY_COORDS);  // Prevents the game from teleporting you
        add_native_detour(RAGE_JOAAT("maintransition"), 0x1A9205C1B9EE827F, maintransition::SET_ENTITY_COLLISION); // Prevents you from falling
        add_native_detour(RAGE_JOAAT("maintransition"), 0xEA1C610A04DB6BBB, maintransition::SET_ENTITY_VISIBLE);  // Makes you visible
        add_native_detour(RAGE_JOAAT("maintransition"), 0x8D32347D6D4C40A2, maintransition::SET_PLAYER_CONTROL); // Allows controll in session switch
        add_native_detour(RAGE_JOAAT("maintransition"), 0x428CA6DBD1094446, maintransition::FREEZE_ENTITY_POSITION); // Allows controll in session switch
        add_native_detour(RAGE_JOAAT("maintransition"), 0x198F77705FA0931D, maintransition::SET_FOCUS_ENTITY); // Prevets map from unloading.
        add_native_detour(RAGE_JOAAT("maintransition"), 0x719FF505F097FD20, maintransition::HIDE_HUD_AND_RADAR_THIS_FRAME); // Draw hud and radar in transition. (Doesn't work.)
        add_native_detour(RAGE_JOAAT("maintransition"), 0xEF01D36B9C9D0C7B, maintransition::ACTIVATE_FRONTEND_MENU); // Let's you controll your ped when going sp to mp.
        add_native_detour(RAGE_JOAAT("maintransition"), 0x10706DC6AD2D49C0, maintransition::RESTART_FRONTEND_MENU); // Let's you controll your ped when going sp to mp.
        add_native_detour(RAGE_JOAAT("maintransition"), 0xDFC252D8A3E15AB7, maintransition::TOGGLE_PAUSED_RENDERPHASES); // Prevents the game from freezing your screen.
        add_native_detour(RAGE_JOAAT("maintransition"), 0xEA23C49EAA83ACFB, maintransition::NETWORK_RESURRECT_LOCAL_PLAYER); // Prevents player from teleporting after switch.
        add_native_detour(RAGE_JOAAT("maintransition"), 0xDC38CC1E35B6A5D7, maintransition::SET_WARNING_MESSAGE_WITH_HEADER); // Bad fix for infinite loading warning message.
        add_native_detour(RAGE_JOAAT("freemode"), 0x5E9564D8246B909A, freemode::IS_PLAYER_PLAYING);
        add_native_detour(RAGE_JOAAT("shop_controller"), 0xDC38CC1E35B6A5D7, shop_controller::SET_WARNING_MESSAGE_WITH_HEADER);

        for (auto& entry : *g_pointers->m_script_program_table)
            if (entry.m_program)
                hook_program(entry.m_program);

        g_native_hooks = this;
    }

    native_hooks::~native_hooks()
    {
        m_script_hooks.clear();
        g_native_hooks = nullptr;
    }

    void native_hooks::add_native_detour(rage::scrNativeHash hash, rage::scrNativeHandler detour)
	{
        add_native_detour(ALL_SCRIPT_HASH, hash, detour);
	}

	void native_hooks::add_native_detour(rage::joaat_t script_hash, rage::scrNativeHash hash, rage::scrNativeHandler detour)
	{
		if (const auto& it = m_native_registrations.find(script_hash); it != m_native_registrations.end())
		{
			it->second.emplace_back(hash, detour);
			return;
		}

		m_native_registrations.emplace(script_hash, std::vector<native_detour>({ { hash, detour } }));
	}

    void native_hooks::hook_program(rage::scrProgram* program)
    {
        std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements;
        const auto script_hash = program->m_name_hash;

        // Functions that need to be detoured for all scripts
        if (const auto& pair = m_native_registrations.find(ALL_SCRIPT_HASH); pair != m_native_registrations.end())
            for (const auto& native_hook_reg : pair->second)
                native_replacements.insert(native_hook_reg);

        // Functions that only need to be detoured for a specific script
        if (const auto& pair = m_native_registrations.find(script_hash); pair != m_native_registrations.end())
            for (const auto& native_hook_reg : pair->second)
                native_replacements.insert(native_hook_reg);

        if (!native_replacements.empty())
        {
            m_script_hooks.emplace(
                program,
                std::make_unique<script_hook>(program, native_replacements)
            );
        }
    }

    void native_hooks::unhook_program(rage::scrProgram* program)
    {
        m_script_hooks.erase(program);
    }
}
