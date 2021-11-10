#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "player_tabs.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "util/toxic.hpp"

namespace big
{
	void tab_player::tab_toxic()
	{
		if (ImGui::BeginTabItem("Toxic"))
		{
			if (ImGui::Button("Explode Self"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::blame_explode_player(g.selected_player.id, g.selected_player.id, eExplosionType::PLANE, 1000, false, true, 0.f);
				}QUEUE_JOB_END_CLAUSE
			}
			if (ImGui::Button("bounty_player"))
			{
				g_fiber_pool->queue_job([]
				{
					toxic::bounty_player(g.selected_player.id, g.selected_player.id, 5000);
				});
			}

			if (ImGui::Button("ceo_kick"))
			{
				g_fiber_pool->queue_job([]
					{
						toxic::ceo_kick(g.selected_player.id);
					});
			}

			if (ImGui::Button("kick"))
			{
				g_fiber_pool->queue_job([]
					{
						toxic::kick(g.selected_player.id);
					});
			}	
			if (ImGui::Button("sens To Brazil"))
			{
				g_fiber_pool->queue_job([]
					{
						toxic::send_island(g.selected_player.id);
					});
			}	

			ImGui::EndTabItem();
		}
	}
}