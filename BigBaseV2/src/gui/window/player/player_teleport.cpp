#include "player_tabs.hpp"
#include "fiber_pool.hpp"
#include "util/teleport.hpp"

namespace big
{
	void tab_player::tab_teleport()
	{
		if (ImGui::BeginTabItem("Teleport"))
		{
			if (ImGui::Button("Teleport"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					teleport::to_player(g.selected_player.id);
				}QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Teleport into Vehicle"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g.selected_player.id), false);

					teleport::into_vehicle(veh);
				}QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Parachute"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Player player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g.selected_player.id);
					teleport::parachute_to_entity(player);
				}QUEUE_JOB_END_CLAUSE
			}

			
			if (ImGui::Button("Bring"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					teleport::bring_player(g.selected_player.id);
				}QUEUE_JOB_END_CLAUSE
			} //godmode_kill

			ImGui::SameLine();

			if (ImGui::Button("Godmode kill"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					teleport::godmode_kill(g.selected_player.id);
				}QUEUE_JOB_END_CLAUSE
			}



			ImGui::EndTabItem();
		}
	}
}