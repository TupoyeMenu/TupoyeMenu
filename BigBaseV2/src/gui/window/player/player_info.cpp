#include "player_tabs.hpp"
#include "fiber_pool.hpp"
#include "script.hpp"
#include "natives.hpp"

namespace big
{
	void tab_player::tab_info()
	{
		if (ImGui::BeginTabItem("Info"))
		{
			ImGui::Checkbox("Spectate", &g.player.spectating);

			if (ImGui::Button("Open Profile"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					int iNetworkHandle[13];
					NETWORK::NETWORK_HANDLE_FROM_PLAYER(g.selected_player.id, iNetworkHandle, 13);
					NETWORK::NETWORK_SHOW_PROFILE_UI(iNetworkHandle);
				}QUEUE_JOB_END_CLAUSE
			}

			ImGui::Separator();

			ImGui::Text("Player ID: %d", g.selected_player.id);

			CNetGamePlayer* net_player = g.selected_player.net_player;
			if (net_player != nullptr)
			{
				CPlayerInfo* player_info = net_player->player_info;
				if (player_info != nullptr)
				{
					ImGui::Text("Session Host: %s", net_player->is_host() ? "Yes" : "No");

					ImGui::Text("Rockstar ID: %d", player_info->m_rockstar_id);
					ImGui::Text("IP Address: %d.%d.%d.%d:%d", player_info->m_external_ip.m_field1, player_info->m_external_ip.m_field2, player_info->m_external_ip.m_field3, player_info->m_external_ip.m_field4, player_info->m_external_port);
				}
			}

			ImGui::EndTabItem();
		}
	}
}