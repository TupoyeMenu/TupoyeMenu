#include "main_tabs.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "pointers.hpp"
#include "util/vehicle.hpp"

namespace big
{
	static char model[255];
	static char prop_name[255];
	static char ped_name[255];

	void tab_main::tab_spawn()
	{
		if (ImGui::BeginTabItem("Spawn"))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			}QUEUE_JOB_END_CLAUSE

			if (
				ImGui::InputText("Vehicle Name", model, sizeof(model), ImGuiInputTextFlags_EnterReturnsTrue) ||
				ImGui::Button("Spawn Vehicle")
				)
			{
				QUEUE_JOB_BEGIN_CLAUSE(= )
				{
					Ped player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX());
					Vector3 location = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, .0, 8.0, .5);

					vehicle::spawn((const char*)model, location, ENTITY::GET_ENTITY_HEADING(player) + 90.f);
				}QUEUE_JOB_END_CLAUSE
			}

			
			if (
				ImGui::InputText("Prop Name", prop_name, sizeof(prop_name), ImGuiInputTextFlags_EnterReturnsTrue) ||
				ImGui::Button("Spawn Prop By Name")
				)
			{
				QUEUE_JOB_BEGIN_CLAUSE(= )
				{
					Ped player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX());
					Vector3 location = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, .0, 8.0, .5);
					entity::spawn_prop(prop_name, location);
				}
				QUEUE_JOB_END_CLAUSE
			}

			if (
				ImGui::InputText("Ped Name", ped_name, sizeof(ped_name), ImGuiInputTextFlags_EnterReturnsTrue) ||
				ImGui::Button("Spawn Ped By Name")
				)
			{
				QUEUE_JOB_BEGIN_CLAUSE(= )
				{
					Ped player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX());
					Vector3 location = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, .0, 8.0, .5);
					entity::spawn_ped(ped_name, location);
				}
				QUEUE_JOB_END_CLAUSE
			}

			ImGui::EndTabItem();
		}
	}
}