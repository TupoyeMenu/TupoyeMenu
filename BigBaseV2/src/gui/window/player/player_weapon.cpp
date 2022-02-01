#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "player_tabs.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "util/toxic.hpp"

namespace big
{
	void tab_player::tab_player_weapon()
	{
		if (ImGui::BeginTabItem("Weapon"))
		{
			static char weapon_name[255];
			ImGui::InputText("Weapon name", weapon_name, 255);

			ImGui::Separator();

			if (ImGui::Button("Give Weapon"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Hash hash = MISC::GET_HASH_KEY(weapon_name);
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->m_selected_player->id()), hash, 1000, 1);
				}QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove Weapon"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Hash hash = MISC::GET_HASH_KEY(weapon_name);
					WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->m_selected_player->id()), hash);
				}QUEUE_JOB_END_CLAUSE
			}
			if (ImGui::Button("Remove Corret Weapon"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->m_selected_player->id());
					Hash hash = WEAPON::GET_SELECTED_PED_WEAPON(ped);
					WEAPON::REMOVE_WEAPON_FROM_PED(ped, hash);
				}QUEUE_JOB_END_CLAUSE
			}
			ImGui::EndTabItem();
		}
	}
}