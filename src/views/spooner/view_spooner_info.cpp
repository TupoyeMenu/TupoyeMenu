#include "fiber_pool.hpp"
#include "pointers.hpp"
#include "widgets/imgui_bitfield.hpp"
#include "views/view.hpp"
#include "util/entity.hpp"
#include "util/local_player.hpp"
#include "util/scripts.hpp"
#include "util/misc.hpp"
#include "services/object_spooner/object_spooner_service.hpp"
#include <vehicle/CVehicleModelInfo.hpp>

namespace big
{
	void view::spooner_info()
	{
		Entity entity_handle = g_object_spooner_service->selected_entity.m_entity_handle;
		CObject* cobject = g_object_spooner_service->selected_entity.m_entity;
		static Vector3 entity_rotation;
		static bool is_a_misson_entity = false;

		ImGui::Text("Info about: #%i", entity_handle);
		ImGui::Text("Class ptr: 0x%X", cobject);
		
		if(cobject != nullptr)
		{
			ImGui::Text("Model Hash: #%u", cobject->m_model_info->m_hash);
			if(g_object_spooner_service->selected_entity.m_is_entity_a_vehicle)
			{
				CVehicleModelInfo* vehicle_model_info = (CVehicleModelInfo*)cobject->m_model_info;
				ImGui::Text("Model Name: %s", vehicle_model_info->m_name);
			}

			g_fiber_pool->queue_job([entity_handle] 
			{
				entity_rotation = ENTITY::GET_ENTITY_ROTATION(entity_handle, 2);
				is_a_misson_entity = ENTITY::IS_ENTITY_A_MISSION_ENTITY(entity_handle);
			});

			components::button("Delete", [entity_handle]
			{
				entity::delete_entity(entity_handle);
			});

			if(ImGui::Checkbox("Is Mission Entity", &is_a_misson_entity))
			{
				g_fiber_pool->queue_job([entity_handle]
				{
					if(is_a_misson_entity)
						ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&g_object_spooner_service->selected_entity.m_entity_handle);
					else
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(entity_handle, false, false);
				});
			}

			bool is_frozen = (cobject->gap2D[1] & 1 << 1);
			if(ImGui::Checkbox("Is Frozen", &is_frozen))
			{
				g_fiber_pool->queue_job([is_frozen, entity_handle]
				{
					ENTITY::FREEZE_ENTITY_POSITION(entity_handle, is_frozen);
				});
			}

			ImGui::Bitfield("Invisible", &cobject->m_invisible); // Use last bit to toggle.

			ImGui::Text("Coords");
			
			if(ImGui::DragFloat("X ##coords_x", &cobject->get_position()->x, 0.5))
			{
				g_fiber_pool->queue_job([cobject, entity_handle]
				{
					ENTITY::SET_ENTITY_COORDS(entity_handle, cobject->get_position()->x, cobject->get_position()->y, cobject->get_position()->z, false, true, true, false);
				});
			}
			if(ImGui::DragFloat("Y ##coords_y", &cobject->get_position()->y, 0.5))
			{
				g_fiber_pool->queue_job([cobject, entity_handle]
				{
					ENTITY::SET_ENTITY_COORDS(entity_handle, cobject->get_position()->x, cobject->get_position()->y, cobject->get_position()->z, false, true, true, false);
				});
			}
			if(ImGui::DragFloat("Z ##coords_z", &cobject->get_position()->z, 0.5))
			{
				g_fiber_pool->queue_job([cobject, entity_handle]
				{
					ENTITY::SET_ENTITY_COORDS(entity_handle, cobject->get_position()->x, cobject->get_position()->y, cobject->get_position()->z, false, true, true, false);
				});
			}

			ImGui::Text("Rotation");

			if(ImGui::SliderFloat("X ##rotation_x", &entity_rotation.x, -180.f, 180.f))
			{
				g_fiber_pool->queue_job([entity_handle]
				{
					ENTITY::SET_ENTITY_ROTATION(entity_handle, entity_rotation.x, entity_rotation.y, entity_rotation.z, 2, false);
				});
			}
			if(ImGui::SliderFloat("Y ##rotation_y", &entity_rotation.y, -180.f, 180.f))
			{
				g_fiber_pool->queue_job([entity_handle]
				{
					ENTITY::SET_ENTITY_ROTATION(entity_handle, entity_rotation.x, entity_rotation.y, entity_rotation.z, 2, false);
				});
			}
			if(ImGui::SliderFloat("Z ##rotation_z", &entity_rotation.z, -180.f, 180.f))
			{
				g_fiber_pool->queue_job([entity_handle]
				{
					ENTITY::SET_ENTITY_ROTATION(entity_handle, entity_rotation.x, entity_rotation.y, entity_rotation.z, 2, false);
				});
			}

			ImGui::SliderFloat("Max Health", &cobject->m_maxhealth, 0.f, 10'000.f);
			ImGui::SliderFloat("Health", &cobject->m_health, 0.f, cobject->m_maxhealth);

			bool is_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::GOD);
			if(ImGui::Checkbox("God Mode", &is_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::GOD;

			ImGui::BeginGroup();

			bool bullet_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::BULLET);
			if(ImGui::Checkbox("Bullet", &bullet_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::BULLET;

			bool fire_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::FIRE);
			if(ImGui::Checkbox("Fire", &fire_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::FIRE;

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			bool collision_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::COLLISION);
			if(ImGui::Checkbox("Collision", &collision_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::COLLISION;

			bool melee_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::MELEE);
			if(ImGui::Checkbox("Melee", &melee_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::MELEE;

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			bool explosion_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::EXPLOSION);
			if(ImGui::Checkbox("Explosion", &explosion_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::EXPLOSION;

			bool steam_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::STEAM);
			if(ImGui::Checkbox("Steam", &steam_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::STEAM;

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			bool drown_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::DROWN);
			if(ImGui::Checkbox("Drown", &drown_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::DROWN;

			bool water_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::WATER);
			if(ImGui::Checkbox("Water", &water_god))
				cobject->m_damage_bits ^= (uint32_t)eEntityProofs::WATER;

			ImGui::EndGroup();

			if(*g_pointers->m_is_session_started && cobject->m_net_object != nullptr)
			{
				ImGui::Text("Net Object Type: %i", cobject->m_net_object->m_object_type);
				ImGui::Text("Net Object ID: %i", cobject->m_net_object->m_object_id);
				ImGui::Text("Net Owner ID: %i", cobject->m_net_object->m_owner_id);
				ImGui::Text("Net Control ID: %i", cobject->m_net_object->m_control_id);
				ImGui::Checkbox("m_is_remote", &cobject->m_net_object->m_is_remote);
				ImGui::Checkbox("m_wants_to_delete", &cobject->m_net_object->m_wants_to_delete);
				ImGui::Checkbox("m_should_not_be_delete", &cobject->m_net_object->m_should_not_be_delete);
			}
		}
	}
}
