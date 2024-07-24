/**
 * @file view_gta_data.cpp
 * @brief GTA Data update window that comes up when cache needs rebuilding.
 */

#include "gui.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "view.hpp"

namespace big
{
	void view::gta_data()
	{
		if (!g_gta_data_service || !g.settings.onboarding_complete)
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
				ImGui::TextUnformatted("TupoyeMenu requires a rebuild of the game cache. This may take up to one minute to generate.");

				if (ImGui::Button("Update Cache"))
				{
					g_gta_data_service->update_now();
				}

				break;
			}
			case eGtaDataUpdateState::WAITING_FOR_SINGLE_PLAYER:
			{
				ImGui::TextUnformatted("Waiting for single player to be loaded...");

				break;
			}
			case eGtaDataUpdateState::UPDATING:
			{
				ImGui::TextUnformatted("Updating cache, please wait...");

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
