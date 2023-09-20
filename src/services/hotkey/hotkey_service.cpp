/**
 * @file hotkey_service.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hotkey_service.hpp"

#include "fiber_pool.hpp"
#include "gui.hpp"
#include "network/ChatData.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "util/teleport.hpp"

namespace big
{
	hotkey_service::hotkey_service()
	{
		// ordered alphabetically to more easily see if a certain hotkey is present
		register_hotkey("bringpv", g.settings.hotkeys.bringvehicle, RAGE_JOAAT("bringpv"));
		register_hotkey("clearwantedlvl", g.settings.hotkeys.clear_wanted, RAGE_JOAAT("clearwantedlvl"));
		register_hotkey("cmdexecutor", g.settings.hotkeys.cmd_excecutor, RAGE_JOAAT("cmdexecutor"));
		register_hotkey("fastquit", g.settings.hotkeys.fast_quit, RAGE_JOAAT("fastquit"));
		register_hotkey("fastrun", g.settings.hotkeys.superrun, RAGE_JOAAT("fastrun"));
		register_hotkey("fillammo", g.settings.hotkeys.fill_ammo, RAGE_JOAAT("fillammo"));
		register_hotkey("fillsnacks", g.settings.hotkeys.fill_inventory, RAGE_JOAAT("fillsnacks"));
		register_hotkey("heal", g.settings.hotkeys.heal, RAGE_JOAAT("heal"));
		register_hotkey("invis", g.settings.hotkeys.invis, RAGE_JOAAT("invis"));
		register_hotkey("invisveh", g.settings.hotkeys.invisveh, RAGE_JOAAT("invisveh"));
		register_hotkey("localinvisveh", g.settings.hotkeys.localinvisveh, RAGE_JOAAT("localinvisveh"));
		register_hotkey("noclip", g.settings.hotkeys.noclip, RAGE_JOAAT("noclip"));
		register_hotkey("objective", g.settings.hotkeys.teleport_objective, RAGE_JOAAT("objectivetp"));
		register_hotkey("passive", g.settings.hotkeys.passive, RAGE_JOAAT("passive"));
		//register_hotkey("repairpv", g.settings.hotkeys.repairpv, RAGE_JOAAT("repairpv"));
		register_hotkey("skipcutscene", g.settings.hotkeys.skip_cutscene, RAGE_JOAAT("skipcutscene"));
		register_hotkey("pvtp", g.settings.hotkeys.teleport_pv, RAGE_JOAAT("pvtp"));
		register_hotkey("vehiclefly", g.settings.hotkeys.vehicle_flymode, RAGE_JOAAT("vehiclefly"));
		register_hotkey("waypoint", g.settings.hotkeys.teleport_waypoint, RAGE_JOAAT("waypointtp"));
		register_hotkey("highlighttp", g.settings.hotkeys.teleport_selected, RAGE_JOAAT("highlighttp"));

		g_renderer->add_wndproc_callback([this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
			wndproc(static_cast<eKeyState>(msg), wparam);
		});

		g_hotkey_service = this;
	}

	hotkey_service::~hotkey_service()
	{
		g_hotkey_service = nullptr;
	}

	void hotkey_service::register_hotkey(const std::string_view name, key_t key, rage::joaat_t command_hash, eKeyState state, std::optional<std::chrono::high_resolution_clock::duration> cooldown)
	{
		m_hotkeys[state == eKeyState::RELEASE].emplace(key, hotkey(rage::joaat(name), key, command_hash, cooldown));
	}

	bool hotkey_service::update_hotkey(const std::string_view name, const key_t key)
	{
		static auto update_hotkey_map = [](hotkey_map& hotkey_map, rage::joaat_t name_hash, key_t new_key) -> bool {
			bool processed = false;

			for (auto it = hotkey_map.begin(); it != hotkey_map.end(); ++it)
			{
				auto hotkey = it->second;
				if (hotkey.name_hash() != name_hash)
					continue;

				hotkey_map.erase(it);
				hotkey.set_key(new_key);
				hotkey_map.emplace(new_key, hotkey);

				processed = true;
			}
			return processed;
		};

		const auto name_hash = rage::joaat(name);
		return update_hotkey_map(m_hotkeys[1], name_hash, key)  // released
		    && update_hotkey_map(m_hotkeys[0], name_hash, key); // down
	}

	void hotkey_service::wndproc(eKeyState state, key_t key)
	{
		if (const auto chat_data = *g_pointers->m_gta.m_chat_data; chat_data && (chat_data->m_chat_open || chat_data->m_timer_two))
			return;

		//command executer is opened
		if (g.cmd_executor.enabled)
			return;

		if (*g_pointers->m_gta.m_is_social_club_overlay_active)
			return;

		if (state == eKeyState::RELEASE || state == eKeyState::DOWN)
		{
			auto& hotkey_map = m_hotkeys[state == eKeyState::RELEASE];
			for (auto [ it, end ] = hotkey_map.equal_range(key); it != end; ++it)
			{
				if (auto& hotkey = it->second; hotkey.can_exec())
				{
					g_fiber_pool->queue_job([&hotkey] {
						hotkey.exec();
					});
				}
			}
		}
	}
}