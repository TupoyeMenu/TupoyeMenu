/**
 * @file byte_patch_manager.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "byte_patch_manager.hpp"

#include "gta/net_array.hpp"
#include "hooking/hooking.hpp"
#include "memory/byte_patch.hpp"
#include "pointers.hpp"
#include "util/explosion_anti_cheat_bypass.hpp"
#include "util/police.hpp"
#include "util/vehicle.hpp"
#include "util/world_model.hpp"

extern "C" void sound_overload_detour();
uint64_t g_sound_overload_ret_addr;

namespace big
{
	static void init()
	{
		// Restore max wanted level after menu unload
		police::m_max_wanted_level =
		    memory::byte_patch::make(g_pointers->m_gta.m_max_wanted_level.add(5).rip().as<uint32_t*>(), 0).get();
		police::m_max_wanted_level_2 =
		    memory::byte_patch::make(g_pointers->m_gta.m_max_wanted_level.add(14).rip().as<uint32_t*>(), 0).get();

		// Patch World Model Spawn Bypass
		std::array<uint8_t, 24> world_spawn_patch;
		std::fill(world_spawn_patch.begin(), world_spawn_patch.end(), 0x90);
		world_model_bypass::m_world_model_spawn_bypass =
		    memory::byte_patch::make(g_pointers->m_gta.m_world_model_spawn_bypass, world_spawn_patch).get();

		// Patch blocked explosions
		explosion_anti_cheat_bypass::m_can_blame_others =
		    memory::byte_patch::make(g_pointers->m_gta.m_blame_explode.as<uint16_t*>(), 0xE990).get();
		explosion_anti_cheat_bypass::m_can_use_blocked_explosions =
		    memory::byte_patch::make(g_pointers->m_gta.m_explosion_patch.sub(12).as<uint16_t*>(), 0x9090).get();

		// Skip matchmaking session validity checks
		memory::byte_patch::make(g_pointers->m_gta.m_is_matchmaking_session_valid.as<void*>(), std::to_array({0xB0, 0x01, 0xC3}))
		    ->apply(); // has no observable side effects

		// Bypass netarray buffer cache when enabled
		broadcast_net_array::m_patch =
		    memory::byte_patch::make(g_pointers->m_gta.m_broadcast_patch.as<uint8_t*>(), 0xEB).get();

		// Disable cheat activated netevent when creator warping
		memory::byte_patch::make(g_pointers->m_gta.m_creator_warp_cheat_triggered_patch.as<uint8_t*>(), 0xEB)->apply();

		// Setup inline hook for sound overload crash protection
		g_sound_overload_ret_addr = g_pointers->m_gta.m_sound_overload_detour.add(13 + 15).as<decltype(g_sound_overload_ret_addr)>();
		std::vector<byte> bytes = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90}; // far jump opcode + a nop opcode
		*(void**)(bytes.data() + 6) = (void*)sound_overload_detour;
		memory::byte_patch::make(g_pointers->m_gta.m_sound_overload_detour.add(13).as<void*>(), bytes)->apply();

		// Disable collision when enabled
		vehicle::disable_collisions::m_patch =
		    memory::byte_patch::make(g_pointers->m_gta.m_disable_collision.sub(2).as<uint8_t*>(), 0xEB).get();

		// Crash Trigger
		memory::byte_patch::make(g_pointers->m_gta.m_crash_trigger.add(4).as<uint8_t*>(), 0x00)->apply();

		// Script VM patches

		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_1.add(2).as<uint32_t*>(), 0xc9310272)->apply();
		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_1.add(6).as<uint16_t*>(), 0x9090)->apply();

		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_2.add(2).as<uint32_t*>(), 0xc9310272)->apply();
		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_2.add(6).as<uint16_t*>(), 0x9090)->apply();

		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_3.add(2).as<uint32_t*>(), 0xd2310272)->apply();
		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_3.add(6).as<uint16_t*>(), 0x9090)->apply();

		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_4.add(2).as<uint32_t*>(), 0xd2310272)->apply();
		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_4.add(6).as<uint16_t*>(), 0x9090)->apply();

		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_5.add(2).as<uint32_t*>(), 0xd2310272)->apply();
		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_5.add(6).as<uint16_t*>(), 0x9090)->apply();

		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_6.add(2).as<uint32_t*>(), 0xd2310272)->apply();
		memory::byte_patch::make(g_pointers->m_gta.m_script_vm_patch_6.add(6).as<uint16_t*>(), 0x9090)->apply();

		// Patch script network check
		memory::byte_patch::make(g_pointers->m_gta.m_model_spawn_bypass, std::vector{0x90, 0x90})->apply(); // this is no longer integrity checked

		// window hook: pt1
		memory::byte_patch::make(g_pointers->m_gta.m_window_hook.as<void*>(), std::to_array({0xC3, 0x90, 0x90, 0x90}))->apply();

		// Prevent the game from crashing when flooded with outgoing events
		memory::byte_patch::make(g_pointers->m_gta.m_free_event_error, std::vector{0x90, 0x90, 0x90, 0x90, 0x90})->apply();

		// Prevent the attribute task from failing
#ifdef ENABLE_SOCIALCLUB
		memory::byte_patch::make(g_pointers->m_sc.m_read_attribute_patch, std::vector{0x90, 0x90})->apply();
		memory::byte_patch::make(g_pointers->m_sc.m_read_attribute_patch_2, std::vector{0xB0, 0x01})->apply();
#endif // ENABLE_SOCIALCLUB

		// Always send the special ability event
		memory::byte_patch::make(g_pointers->m_gta.m_activate_special_ability_patch, std::to_array({0xB0, 0x01, 0xC3}))->apply();
	}

	byte_patch_manager::byte_patch_manager()
	{
		init();

		g_byte_patch_manager = this;
	}

	byte_patch_manager::~byte_patch_manager()
	{
		memory::byte_patch::restore_all();

		g_byte_patch_manager = nullptr;
	}
}
