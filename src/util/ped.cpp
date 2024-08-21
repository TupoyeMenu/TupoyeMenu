#include "util/ped.hpp"

#include "gta/joaat.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "services/players/player_service.hpp"
#include "util/entity.hpp"
#include "util/math.hpp"
#include "util/outfit.hpp"
#include "util/vehicle.hpp"


namespace big::ped
{
	void cage_ped(Ped ped)
	{
		Hash hash = "prop_gold_cont_01"_J;

		Vector3 location = ENTITY::GET_ENTITY_COORDS(ped, true);
		OBJECT::CREATE_OBJECT(hash, location.x, location.y, location.z - 1.f, true, false, false);
	}

	void clean_ped(Ped ped)
	{
		Ped player_ped = self::ped;

		PED::CLEAR_PED_BLOOD_DAMAGE(player_ped);
		PED::CLEAR_PED_WETNESS(player_ped);
		PED::CLEAR_PED_ENV_DIRT(player_ped);
		PED::RESET_PED_VISIBLE_DAMAGE(player_ped);
	}

	bool change_player_model(const Hash hash)
	{
		if (entity::request_model(hash))
		{
			self::ped = PLAYER::PLAYER_PED_ID();
			PLAYER::SET_PLAYER_MODEL(self::id, hash);
			script::get_current()->yield();
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			for (int i = 0; i < 12; i++)
			{
				PED::SET_PED_COMPONENT_VARIATION(self::ped, i, PED::GET_PED_DRAWABLE_VARIATION(self::ped, i), PED::GET_PED_TEXTURE_VARIATION(self::ped, i), PED::GET_PED_PALETTE_VARIATION(self::ped, i));
			}
			return true;
		}
		return false;
	}

	bool steal_outfit(const Ped target)
	{
		Ped ped = self::ped;

		if (ENTITY::GET_ENTITY_MODEL(ped) != ENTITY::GET_ENTITY_MODEL(target))
		{
			return false;
		}
		for (int i = 0; i < 12; i++)
		{
			PED::SET_PED_COMPONENT_VARIATION(ped, i, PED::GET_PED_DRAWABLE_VARIATION(target, i), PED::GET_PED_TEXTURE_VARIATION(target, i), PED::GET_PED_PALETTE_VARIATION(target, i));
		}

		return true;
	}

	void clone_ped(const Ped src, const Ped target)
	{
		PED::CLONE_PED_TO_TARGET(src, target);
		auto src_ptr = g_pointers->m_gta.m_handle_to_ptr(src);
		auto dst_ptr = g_pointers->m_gta.m_handle_to_ptr(target);

		if (src_ptr && dst_ptr)
		{
			for (auto container = src_ptr->m_extension_container; container; container = container->m_next)
			{
				if (container->m_entry && container->m_entry->get_id() == 0xB)
				{
					g_pointers->m_gta.m_set_head_blend_data(reinterpret_cast<CPed*>(dst_ptr),
					    reinterpret_cast<CHeadBlendData*>(container->m_entry));
					break;
				}
			}
		}
	}

	void steal_identity(const Ped target)
	{
		const int max_health     = ENTITY::GET_ENTITY_MAX_HEALTH(self::ped);
		const int current_health = ENTITY::GET_ENTITY_HEALTH(self::ped);
		const int current_armor  = PED::GET_PED_ARMOUR(self::ped);

		if (ENTITY::GET_ENTITY_MODEL(target) != ENTITY::GET_ENTITY_MODEL(self::id))
		{
			PLAYER::SET_PLAYER_MODEL(self::id, ENTITY::GET_ENTITY_MODEL(target));
			script::get_current()->yield();
		}
		clone_ped(target, self::ped);
		ENTITY::SET_ENTITY_MAX_HEALTH(self::ped, max_health);
		ENTITY::SET_ENTITY_HEALTH(self::ped, current_health, 0, 0);
		PED::SET_PED_ARMOUR(self::ped, current_armor);
	}

	void kill_ped(const Ped ped)
	{
		if (entity::take_control_of(ped, 0))
			ENTITY::SET_ENTITY_HEALTH(ped, 0, self::ped, 0);
		else
		{
			auto ptr = g_pointers->m_gta.m_handle_to_ptr(ped);
			if (!ptr)
				return;

			g_pointers->m_gta.m_send_network_damage(g_player_service->get_self()->get_ped(), ptr, ptr->get_position(), 0, true, "weapon_explosion"_J, 10000.0f, 2, 0, (1 << 4), 0, 0, 0, false, false, true, true, nullptr);
		}
	}

	void kill_ped_by_relation(Ped ped, int relation_id)
	{
		if (PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, PLAYER::PLAYER_PED_ID()) == relation_id)
			kill_ped(ped);
	}

	Ped spawn_in_vehicle(Hash hash, Vehicle veh, bool is_networked)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);

			script::get_current()->yield();
		}
		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			g_notification_service.push_warning("Spawn", "Failed to spawn model, did you give an incorrect model?");

			return -1;
		}

		Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(veh, 0, hash, -1, is_networked, false);

		script::get_current()->yield();

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		return ped;
	}


	Ped spawn(ePedType pedType, Hash hash, Ped clone, Vector3 location, float heading, bool is_networked)
	{
		if (entity::request_model(hash))
		{
			Ped ped = PED::CREATE_PED(pedType, hash, location.x, location.y, location.z, heading, is_networked, false);

			script::get_current()->yield();

			if (clone)
			{
				clone_ped(clone, ped);
			}

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

			return ped;
		}
		return 0;
	}

	void set_attacker_ped_flags(Ped ped, Ped ped_to_attack)
	{
		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, "HATES_PLAYER"_J);
		PED::SET_PED_HEARING_RANGE(ped, 5000.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true); // Disables the dumb scripted dying animation.

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true); // Always fight.

		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);          // Don't ragoll from fire and bullets.
		TASK::TASK_COMBAT_PED(ped, ped_to_attack, 0, 16); // 16 allows to fight armed peds.
	}

	Ped spawn_griefer_jesus(Ped ped_to_attack)
	{
		Ped ped;
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped_to_attack, false) + Vector3(0, 0, 1);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped_to_attack, false))
			ped = ped::spawn_in_vehicle(ePedType::PED_TYPE_CRIMINAL, "u_m_m_jesus_01"_J, true);
		else
			ped = ped::spawn(ePedType::PED_TYPE_CRIMINAL, "u_m_m_jesus_01"_J, 0, pos, 0);

		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);
		WEAPON::GIVE_WEAPON_TO_PED(ped, "WEAPON_RAILGUN"_J, 9999, true, true);
		PED::SET_PED_FIRING_PATTERN(ped, "FIRING_PATTERN_FULL_AUTO"_J);
		ped::set_attacker_ped_flags(ped, ped_to_attack);

		return ped;
	}

	Ped spawn_extrime_griefer_jesus(Ped ped_to_attack)
	{
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped_to_attack, false) + Vector3(0, 0, 5);
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(ped_to_attack, false) ? PED::GET_VEHICLE_PED_IS_IN(ped_to_attack, false) : ped_to_attack);

		Vehicle veh = vehicle::spawn("oppressor2"_J, pos, heading, true, false, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		vehicle::max_vehicle(veh);
		ENTITY::SET_ENTITY_PROOFS(veh, false, true, true, false, false, false, false, false); // Fire and explosion proof.

		Ped ped = ped::spawn_in_vehicle("u_m_m_jesus_01"_J, veh, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);   // Never leave the vehicle.
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1); // Disable knock off vehicle.
		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false); // Fire and explosion proof.
		WEAPON::GIVE_WEAPON_TO_PED(ped, "WEAPON_RAILGUN"_J, 9999, true, true);
		PED::SET_PED_FIRING_PATTERN(ped, "FIRING_PATTERN_FULL_AUTO"_J);
		ped::set_attacker_ped_flags(ped, ped_to_attack);

		return ped;
	}

	Ped spawn_griefer_jet(Ped ped_to_attack, Hash vehicle)
	{
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped_to_attack, false) + Vector3(0, 0, 30);
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(ped_to_attack, false) ? PED::GET_VEHICLE_PED_IS_IN(ped_to_attack, false) : ped_to_attack);

		Vehicle veh = vehicle::spawn(vehicle, pos, heading, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		VEHICLE::CONTROL_LANDING_GEAR(veh, 3);

		Ped ped = ped::spawn_in_vehicle("u_m_m_jesus_01"_J, veh, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		TASK::TASK_PLANE_MISSION(ped, veh, PED::GET_VEHICLE_PED_IS_IN(ped_to_attack, false), ped_to_attack, 0, 0, 0, 6, 150.f, 0.f, 0.f, 2500.f, -200.f, -1.f);
		WEAPON::GIVE_WEAPON_TO_PED(ped, "WEAPON_RAILGUN"_J, 9999, true, true);
		PED::SET_PED_FIRING_PATTERN(ped, "FIRING_PATTERN_FULL_AUTO"_J);
		ped::set_attacker_ped_flags(ped, ped_to_attack);

		return ped;
	}

	void set_ped_random_component_variation(Ped ped)
	{
		constexpr auto range = [](int lower_bound, int upper_bound) -> int {
			return std::rand() % (upper_bound - lower_bound + 1) + lower_bound;
		};

		outfit::components_t components;

		for (auto& item : components.items)
		{
			int drawable_id_max = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(ped, item.id) - 1;
			if (drawable_id_max == -1)
				continue;
			int drawable_id    = range(0, drawable_id_max);
			int texture_id_max = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(ped, item.id, drawable_id) - 1;
			if (texture_id_max == -1)
				continue;
			int texture_id = range(0, texture_id_max);
			PED::SET_PED_COMPONENT_VARIATION(ped, item.id, drawable_id, texture_id, PED::GET_PED_PALETTE_VARIATION(ped, item.id));
		}
	}

	player_ptr get_player_from_ped(Ped ped)
	{
		for (auto& p : g_player_service->players())
		{
			if (p.second->get_ped())
			{
				if (p.second->get_ped() == g_pointers->m_gta.m_handle_to_ptr(ped))
					return p.second;
			}
		}
		return nullptr;
	}

	bool load_animation_dict(const char* dict)
	{
		if (STREAMING::HAS_ANIM_DICT_LOADED(dict))
			return true;

		for (uint8_t i = 0; !STREAMING::HAS_ANIM_DICT_LOADED(dict) && i < 35; i++)
		{
			STREAMING::REQUEST_ANIM_DICT(dict);
			script::get_current()->yield();
		}

		return STREAMING::HAS_ANIM_DICT_LOADED(dict);
	}

	void ped_play_animation(Ped ped, const std::string_view& animDict, const std::string_view& animName, float speed, float speedMultiplier, int duration, int flag, float playbackRate, bool lockPos, Vector3 pos, Vector3 rot, int ik_flags)
	{
		if (load_animation_dict(animDict.data()))
		{
			if (pos.x == 0 && pos.y == 0 && pos.z == 0)
				TASK::TASK_PLAY_ANIM(ped, animDict.data(), animName.data(), speed, speedMultiplier, duration, flag, playbackRate, lockPos, lockPos, lockPos);
			else
				TASK::TASK_PLAY_ANIM_ADVANCED(ped, animDict.data(), animName.data(), pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, speed, speedMultiplier, duration, flag, playbackRate, lockPos, ik_flags);
		}
	}

	/*
	* Will make the ped enter the vehicle with animation if vehicle is in vicinity
	* Param movespeed: 1 = walk, 2 = run, 3 = sprint
	*/
	void ped_enter_vehicle_animated(Ped ped, Vehicle veh, eVehicleSeats seat, int movespeed)
	{
		if (entity::take_control_of(ped))
		{
			if (ENTITY::DOES_ENTITY_EXIST(veh))
			{
				if (math::distance_between_vectors(ENTITY::GET_ENTITY_COORDS(ped, 0), ENTITY::GET_ENTITY_COORDS(veh, 0)) < 15.f)
					TASK::TASK_ENTER_VEHICLE(ped, veh, 10000, (int)seat, movespeed, 8, NULL, 0);
				else
					PED::SET_PED_INTO_VEHICLE(ped, veh, (int)seat);
			}
		}
	}

	bool is_ped_a_friend(Ped ped, CPed* ped_ptr)
	{
		if (PED::GET_PED_CONFIG_FLAG(ped, 38, TRUE) == TRUE)
			return false;

		if (PED::IS_PED_IN_COMBAT(ped, self::ped))
			return false;

		return !g_pointers->m_gta.m_is_ped_enemies_with(ped_ptr->m_ped_intelligence, g_local_player, true, false, false);
	}
}