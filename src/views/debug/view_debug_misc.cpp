#include "fiber_pool.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "thread_pool.hpp"
#include "util/system.hpp"
#include "views/view.hpp"

namespace big
{
	void view::debug_misc() 
	{
		components::button("Load MP Map", []
		{
				DLC::ON_ENTER_MP();
		});
		ImGui::SameLine();
		components::button("Load SP Map", []
		{
			DLC::ON_ENTER_SP();
		});

		if (components::button("Dump entrypoints"))
		{
			system::dump_entry_points();
		}
		ImGui::SameLine();
		components::button("Network Bail", []
		{
			NETWORK::NETWORK_BAIL(16, 0, 0);
		});

		components::button("Hide news", []
		{
			SOCIALCLUB::SC_TRANSITION_NEWS_END();
		});
		ImGui::SameLine();
		components::button("Network Session Host", []
		{
			NETWORK::NETWORK_SESSION_HOST(1, 32, false);
		});

		if (g_local_player && g_local_player->m_player_info)
		{
			ImGui::InputScalar("Rockstar ID", ImGuiDataType_S64, &g_local_player->m_player_info->m_net_player_data.m_gamer_handle_2.m_rockstar_id, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
		}

		ImGui::Checkbox("Log Native Script Hooks", &g.debug.logs.script_hook_logs);
		ImGui::Checkbox("Enable Stupid Script Native Logs", &g.debug.logs.stupid_script_native_logs);

		ImGui::Checkbox("Log Packets", &g.debug.logs.packet_logs);
		ImGui::Checkbox("Log Net Events", &g.debug.logs.net_event_logs);

		ImGui::Checkbox("Log Remote Sounds", &g.debug.logs.remote_sound_logs);
		
		ImGui::Checkbox("Log Metrics", &g.debug.logs.metric_logs);

		if (ImGui::TreeNode("Script Event Logging"))
		{
			ImGui::Checkbox("Enable Script Event Logging", &g.debug.logs.script_event.logs);
			ImGui::Separator();

			ImGui::Checkbox("Filter by Player", &g.debug.logs.script_event.filter_player);

			if (g.debug.logs.script_event.filter_player)
			{
				ImGui::ListBoxHeader("##filter_player");
				for (const auto& [_, player] : g_player_service->players())
				{
					if (components::selectable(player->get_name(), g.debug.logs.script_event.player_id == player->id()))
					{
						g.debug.logs.script_event.player_id = player->id();
					}
				}
				ImGui::EndListBox();
			}

			ImGui::TreePop();
		}

		if (components::button("Refresh Interior"))
		{
			Interior interior = INTERIOR::GET_INTERIOR_AT_COORDS(self::pos.x, self::pos.y, self::pos.z);
			INTERIOR::REFRESH_INTERIOR(interior);
		}

		ImGui::SameLine(); components::help_marker("You Will Have To Refresh Again When Exiting Interior.\n SPAMMING WILL CRASH GAME");

		components::command_button<"fastquit">();
	}
}