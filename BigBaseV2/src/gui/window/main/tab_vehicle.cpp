#include "core/data/speedo_meters.hpp"
#include "fiber_pool.hpp"
#include "main_tabs.hpp"
#include "script.hpp"
#include "util/blip.hpp"
#include "util/entity.hpp"
#include "util/notify.hpp"
#include "util/vehicle.hpp"
#include "util/PersistCar.h"

namespace big
{
	static char model[12];

	void tab_main::tab_vehicle()
	{
		if (ImGui::BeginTabItem("Vehicle"))
		{
			if (ImGui::TreeNode("General"))
			{
				ImGui::BeginGroup();
				ImGui::Checkbox("God Mode", &g.vehicle.god_mode);
				ImGui::Checkbox("Horn Boost", &g.vehicle.horn_boost);

				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();

				if (ImGui::Button("Repair"))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

						vehicle::repair(veh);
					}QUEUE_JOB_END_CLAUSE
				}
				if (ImGui::Button("Handling"))
					g.window.handling = true;
				ImGui::EndGroup();

				if (ImGui::Button("Add Explosive"))
				{
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

					vehicle::add_explosive(veh);
				}

				ImGui::SameLine();

				if (ImGui::Button("Detonate Explosive"))
				{
					vehicle::detonate_explosive();
				}

				ImGui::Checkbox("Vehicle Flares", &g.vehicle.flares);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("LS Customs"))
			{
				if (ImGui::Button("Start LS Customs"))
				{
					g.vehicle.ls_customs = true;
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Speedo Meter"))
			{
				SpeedoMeter selected = g.vehicle.speedo_meter.type;

				ImGui::Text("Type:");
				if (ImGui::BeginCombo("###speedo_type", speedo_meters[(int)selected].name))
				{
					for (const speedo_meter& speedo : speedo_meters)
					{
						if (ImGui::Selectable(speedo.name, speedo.id == selected))
						{
							g.vehicle.speedo_meter.type = speedo.id;
						}

						if (speedo.id == selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Text("Position");

				float pos[2];
				pos[0] = g.vehicle.speedo_meter.x;
				pos[1] = g.vehicle.speedo_meter.y;
				if (ImGui::SliderFloat2("###speedo_pos", pos, .001f, .999f, "%.3f"))
				{
					g.vehicle.speedo_meter.x = pos[0];
					g.vehicle.speedo_meter.y = pos[1];
				}

				ImGui::Checkbox("Left Sided", &g.vehicle.speedo_meter.left_side);

				ImGui::TreePop();
			}


			persist_car::do_presentation_layer();

			ImGui::EndTabItem();
		}
	}
}