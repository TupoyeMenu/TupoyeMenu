/**
 * @file view_gta_cache.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "thread_pool.hpp"
#include "views/view.hpp"

namespace big
{
	void view::gta_cache()
	{
		auto ped_count = g_gta_data_service->peds().size();
		auto veh_count = g_gta_data_service->vehicles().size();
		auto wep_count = g_gta_data_service->weapons().size();

		ImGui::Text("GTA cache stats:");
		ImGui::Text("Peds Cached: %d\nVehicles Cached: %d\nWeapons Cached: %d", ped_count, veh_count, wep_count);

		if (components::button("Rebuild Cache in Online"))
		{
			g_gta_data_service->set_state(eGtaDataUpdateState::NEEDS_UPDATE);

			if (!*g_pointers->m_gta.m_is_session_started)
			{
				g_gta_data_service->update_in_online();
			}
			else
			{
				g_gta_data_service->update_now();
			}
		}
	}
}
