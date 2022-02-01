#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "player_tabs.hpp"
#include "natives.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "util/toxic.hpp"

namespace big
{
	void tab_player::tab_player_explosions()
	{
		if (ImGui::BeginTabItem("Explosions"))
		{
			ImGui::Combo("##type", &g.weapons.expl_selected, exlosions, sizeof(exlosions) / sizeof(*exlosions));

			if (ImGui::Button("Spawn explosion"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::blame_explode_player(g_player_service->m_selected_player->id(), g_player_service->m_selected_player->id(), g.weapons.expl_selected, 0, g.weapons.isAudible, g.weapons.isInvisible, 0.f);
				}QUEUE_JOB_END_CLAUSE
			}
			ImGui::SameLine();
			ImGui::Checkbox("isAudible", &g.weapons.isAudible);
			ImGui::SameLine();
			ImGui::Checkbox("isInvisible", &g.weapons.isInvisible);

			ImGui::EndTabItem();
		}
	}
}