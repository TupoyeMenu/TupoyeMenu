#include "fiber_pool.hpp"
#include "main_tabs.hpp"
#include "script.hpp"
#include "util/player.hpp"

namespace big
{
	void tab_main::tab_recovery()
	{
		if (ImGui::BeginTabItem("Recovery"))
		{
			ImGui::Text("Set Current Character Level:");
			ImGui::SliderInt("##input_levels_self", &g.player.set_level, 0, 8000);
			if (ImGui::Button("Set Level"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					player::set_player_level(g.player.set_level);
				}QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button("Max Snacks")) { 
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_YUM_SNACKS"), 99999999, true);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_HEALTH_SNACKS"), 99999999, true); 
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_EPIC_SNACKS"), 99999999, true); 
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CIGARETTES_BOUGHT"), 99999999, true); 
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NUMBER_OF_ORANGE_BOUGHT"), 99999999, true); 
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NUMBER_OF_BOURGE_BOUGHT"), 99999999, true); 
			}

			if (ImGui::Button("Max Beer")) {
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NUMBER_OF_BOURGE_BOUGHT"), 99999999, true);
			}

			ImGui::EndTabItem();
		}
	}
}