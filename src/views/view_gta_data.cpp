/**
 * @file view_gta_data.cpp
 * @brief GTA Data update window that comes up when cache needs rebuilding.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gui.hpp"
#include "pointers.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "view.hpp"

namespace big
{
	void view::gta_data()
	{
		if (!g_gta_data_service)
			return;

		if (g_gta_data_service->cache_needs_update())
		{
			g_gui->toggle(true);
			ImGui::OpenPopup("Game Cache");
		}

		ImGui::SetNextWindowSize({800, 210}, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos({200, 200}, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal("Game Cache"))
		{
			switch (g_gta_data_service->state())
			{
			case eGtaDataUpdateState::NEEDS_UPDATE:
			{
				ImGui::Text("TupoyeMenu requires a rebuild of the game cache. This may take up to one minute to generate.");

				if (*g_pointers->m_gta.m_is_session_started)
				{
					if (ImGui::Button("Update Cache"))
					{
						g_gta_data_service->update_now();
					}
				}
				else
				{
					if (ImGui::Button("Update Cache"))
					{
						g_gta_data_service->update_now();
					}

					if (*g_pointers->m_gta.m_game_state == eGameState::Respawn)
					{
						if (ImGui::Button("Update Cache On Init"))
						{
							g_gta_data_service->update_on_init();
						}
					}
				}

				break;
			}
			case eGtaDataUpdateState::WAITING_FOR_SINGLE_PLAYER:
			{
				ImGui::Text("Waiting for single player to be loaded...");

				break;
			}
			case eGtaDataUpdateState::WAITING_FOR_ONLINE:
			{
				ImGui::Text("Waiting for online to start cache update...");

				break;
			}
			case eGtaDataUpdateState::UPDATING:
			{
				ImGui::Text("Updating cache, please wait...");

				break;
			}
			case eGtaDataUpdateState::ON_INIT_WAITING:
			{
				ImGui::Text("Waiting for single player to be loaded...");

				break;
			}
			case eGtaDataUpdateState::ON_INIT_UPDATE_START:
			{
				ImGui::Text("Updating cache, please wait...");

				break;
			}
			case eGtaDataUpdateState::IDLE:
			{
				ImGui::CloseCurrentPopup();

				break;
			}
			}

			ImGui::EndPopup();
		}
	}
}
