#include "fiber_pool.hpp"
#include "gta/enums.hpp"
#include "gta_util.hpp"
#include "player_tabs.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "util/toxic.hpp"
#include "util/entity.hpp"
#include "util/notify.hpp"

namespace big
{
	void tab_player::tab_toxic()
	{
		if (ImGui::BeginTabItem("Toxic"))
		{

			//ImGui::Combo("##type", &expl_selected, eExplosionType, sizeof(exlosions) / sizeof(*exlosions));

			/*if (ImGui::Button("Explode Self"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::blame_explode_player(
						g_player_service->m_selected_player->id(),
						g_player_service->m_selected_player->id(),
						eExplosionType::PLANE, 1000, false, true, 0.f
					);
				}QUEUE_JOB_END_CLAUSE
			}*/


			if (ImGui::Button("Host Kick"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					NETWORK::NETWORK_SESSION_KICK_PLAYER(g_player_service->m_selected_player->id());
				}
				QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Kick"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::kick(g_player_service->m_selected_player->id());
				}
				QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button("Crash Event"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::crash_event(g_player_service->m_selected_player->id());
				}
				QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Crash (ped)"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Ped player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->m_selected_player->id());
					Vector3 location = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, .0, 8.0, .5);
					Ped ped = entity::spawn_ped("slod_human", location);
					//notify::player_crashing(g_player_service->m_selected_player->id());

					std::this_thread::sleep_for(1000ms);

					notify::above_map("removing ped");
					PED::DELETE_PED(&ped);
				}
				QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button("SoundSpam"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::SoundSpam(g_player_service->m_selected_player->id());
				}
				QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Send To Brazil"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::SendToIsland(g_player_service->m_selected_player->id());
				}
				QUEUE_JOB_END_CLAUSE
			}	

			if (ImGui::Button("10K Baunty"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::bounty_player(g_player_service->m_selected_player->id(), PLAYER::PLAYER_ID(), 10000);
				}
				QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button("Interior"))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					toxic::ForceMission(g_player_service->m_selected_player->id());
				}
				QUEUE_JOB_END_CLAUSE
			}

			ImGui::Checkbox("Freeze", &g.player.freezing);

			/*
			if (ImGui::Button("Teleport To Apartment"))
			{
				g_fiber_pool->queue_job([]
				{
					toxic::Teleport(g_player_service->m_selected_player->id());
				});
			}
			*/



			ImGui::EndTabItem();
		}
	}
}