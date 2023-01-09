#include "fiber_pool.hpp"
#include "pointers.hpp"
#include "util/entity.hpp"
#include "util/local_player.hpp"
#include "views/view.hpp"
#include "util/scripts.hpp"
#include "services/object_spooner/object_spooner_service.hpp"

namespace big
{
	void view::spooner_info()
	{
        Entity entity_handle = g_object_spooner_service->selected_entity.m_entity_handle;
        CObject* cobject = g_object_spooner_service->selected_entity.m_entity;

		ImGui::Text("Info about: #%i", entity_handle);
        
        if(cobject != nullptr)
        {
            ImGui::Text("Model Hash: #%u", cobject->m_model_info->m_hash);
            
            if(ImGui::DragFloat("##coords_x", &cobject->get_position()->x, 0.5))
            {
                g_fiber_pool->queue_job([cobject, entity_handle]
                {
                    ENTITY::SET_ENTITY_COORDS(entity_handle, cobject->get_position()->x, cobject->get_position()->y, cobject->get_position()->z, false, true, true, false);
                });
            }
            if(ImGui::DragFloat("##coords_y", &cobject->get_position()->y, 0.5))
            {
                g_fiber_pool->queue_job([cobject, entity_handle]
                {
                    ENTITY::SET_ENTITY_COORDS(entity_handle, cobject->get_position()->x, cobject->get_position()->y, cobject->get_position()->z, false, true, true, false);
                });
            }
            if(ImGui::DragFloat("##coords_z", &cobject->get_position()->z, 0.5))
            {
                g_fiber_pool->queue_job([cobject, entity_handle]
                {
                    ENTITY::SET_ENTITY_COORDS(entity_handle, cobject->get_position()->x, cobject->get_position()->y, cobject->get_position()->z, false, true, true, false);
                });
            }

            ImGui::SliderFloat("Health", &cobject->m_health, 0.f, cobject->m_maxhealth);

            bool is_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::GOD);
            ImGui::Checkbox("God Mode", &is_god);

            ImGui::BeginGroup();

            bool bullet_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::BULLET);
    		ImGui::Checkbox("Bullet", &bullet_god);
            bool fire_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::FIRE);
    		ImGui::Checkbox("Fire", &fire_god);

    		ImGui::EndGroup();
    		ImGui::SameLine();
    		ImGui::BeginGroup();

            bool collision_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::COLLISION);
    		ImGui::Checkbox("Collision", &collision_god);
            bool melee_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::MELEE);
    		ImGui::Checkbox("Melee", &melee_god);

    		ImGui::EndGroup();
    		ImGui::SameLine();
    		ImGui::BeginGroup();

            bool explosion_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::EXPLOSION);
    		ImGui::Checkbox("Explosion", &explosion_god);
            bool steam_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::STEAM);
    		ImGui::Checkbox("Steam", &steam_god);

    		ImGui::EndGroup();
    		ImGui::SameLine();
    		ImGui::BeginGroup();

            bool drown_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::DROWN);
            bool water_god = (cobject->m_damage_bits & (uint32_t)eEntityProofs::WATER);
    		ImGui::Checkbox("Drown", &drown_god);
    		ImGui::Checkbox("Water", &water_god);

    		ImGui::EndGroup();

            components::button("Menyoo sounds", [entity_handle] {
                AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "Text_Arrive_Tone", entity_handle, "Phone_SoundSet_Default", true, 20);
            });

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
