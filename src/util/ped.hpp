#pragma once
#include "entity.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "vehicle.hpp"
#include "outfit.hpp"

namespace big::ped
{
	inline bool change_player_model(const Hash hash)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}
		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			return false;
		}
		PLAYER::SET_PLAYER_MODEL(self::id, hash);
		self::ped = PLAYER::PLAYER_PED_ID();
		script::get_current()->yield();
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		for (int i = 0; i < 12; i++)
		{
			PED::SET_PED_COMPONENT_VARIATION(self::ped, i, PED::GET_PED_DRAWABLE_VARIATION(self::ped, i), PED::GET_PED_TEXTURE_VARIATION(self::ped, i), PED::GET_PED_PALETTE_VARIATION(self::ped, i));
		}
		return true;
	}

	inline bool steal_outfit(const Ped target)
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

	inline int spawn_in_vehicle(std::string_view model, Vehicle veh, bool is_networked = true, bool clean_up = true)
	{
		if (const Hash hash = rage::joaat(model.data()); hash)
		{
			for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
			{
				STREAMING::REQUEST_MODEL(hash);

				script::get_current()->yield();
			}
			if (!STREAMING::HAS_MODEL_LOADED(hash))
			{
				g_notification_service->push_warning("Spawn", "Failed to spawn model, did you give an incorrect model?");

				return -1;
			}

			Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(veh, 0, hash, -1, is_networked, false);

			script::get_current()->yield();

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

			if (*g_pointers->m_is_session_started)
			{
				DECORATOR::DECOR_SET_INT(ped, "MPBitset", 0);
				ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(ped, clean_up);
			}

			return ped;
		}

		return -1;
	}

	inline void steal_identity(const Ped target)
	{
		const int max_health     = ENTITY::GET_ENTITY_MAX_HEALTH(self::ped);
		const int current_health = ENTITY::GET_ENTITY_HEALTH(self::ped);
		const int current_armor  = PED::GET_PED_ARMOUR(self::ped);

		PLAYER::SET_PLAYER_MODEL(self::id, ENTITY::GET_ENTITY_MODEL(target));
		script::get_current()->yield();
		PED::CLONE_PED_TO_TARGET(target, self::ped);
		ENTITY::SET_ENTITY_MAX_HEALTH(self::ped, max_health);
		ENTITY::SET_ENTITY_HEALTH(self::ped, current_health, 0);
		PED::SET_PED_ARMOUR(self::ped, current_armor);
	}

	inline void kill_ped(const Ped ped)
	{
		if (entity::take_control_of(ped))
			PED::APPLY_DAMAGE_TO_PED(ped, PED::GET_PED_MAX_HEALTH(ped) * 2, false, 0);
	}

	inline void kill_ped_by_relation(Ped ped, int relation_id)
	{
		if (PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, PLAYER::PLAYER_PED_ID()) == relation_id)
			kill_ped(ped);
	}

	inline Ped spawn(ePedType pedType, Hash hash, Hash clone, Vector3 location, float heading, bool is_networked = true)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}

		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			return 0;
		}

		auto ped = PED::CREATE_PED(pedType, hash, location.x, location.y, location.z, heading, is_networked, false);

		script::get_current()->yield();

		if (clone)
		{
			PED::CLONE_PED_TO_TARGET(clone, ped);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		return ped;
	}

	///
	/// GPL-3 CODE STARTS HERE
	/// SKIDDED FROM https://github.com/gta-chaos-mod/ChaosModV
	///

	inline Hash create_bad_ped_relationship_group(std::string group_name)
	{
		static const Hash playerGroup = RAGE_JOAAT("PLAYER");
		static const Hash civGroup    = RAGE_JOAAT("CIVMALE");
		static const Hash femCivGroup = RAGE_JOAAT("CIVFEMALE");

		Hash relationshipGroup;
		PED::ADD_RELATIONSHIP_GROUP(group_name.c_str(), &relationshipGroup);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

		return relationshipGroup;
	}

	inline Ped spawn_griefer_jesus(Vector3 pos, Ped target)
	{
		Hash relationshipGroup = create_bad_ped_relationship_group("_HOSTILE_JESUS");

		Ped ped = ped::spawn(ePedType::PED_TYPE_CRIMINAL, RAGE_JOAAT("u_m_m_jesus_01"), 0, pos, 0);

		if (PED::IS_PED_IN_ANY_VEHICLE(target, false))
		{
			PED::SET_PED_INTO_VEHICLE(ped, PED::GET_VEHICLE_PED_IS_IN(target, false), -2);
		}

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		PED::SET_PED_HEARING_RANGE(ped, 9999.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true);

		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
		PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		TASK::TASK_COMBAT_PED(ped, target, 0, 16);

		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));

		return ped;
	}

	inline Ped spawn_extrime_griefer_jesus(Vector3 pos, Ped target)
	{
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(target, false) ? PED::GET_VEHICLE_PED_IS_IN(target, false) : target);

		Vehicle veh = vehicle::spawn(RAGE_JOAAT("oppressor2"), pos, heading, true, false, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		vehicle::max_vehicle(veh);
		ENTITY::SET_ENTITY_PROOFS(veh, false, true, true, false, false, false, false, false);

		Hash relationshipGroup = create_bad_ped_relationship_group("_HOSTILE_JESUS");

		Ped ped = ped::spawn_in_vehicle("u_m_m_jesus_01", veh, true);

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		PED::SET_PED_HEARING_RANGE(ped, 9999.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true);

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
		PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		TASK::TASK_COMBAT_PED(ped, target, 0, 16);

		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));

		return ped;
	}

	inline Ped spawn_griefer_jet(Vector3 pos, Ped target, Hash vehicle)
	{
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(target, false) ? PED::GET_VEHICLE_PED_IS_IN(target, false) : target);

		Vehicle veh = vehicle::spawn(vehicle, pos, heading, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		VEHICLE::CONTROL_LANDING_GEAR(veh, 3);

		Hash relationshipGroup = create_bad_ped_relationship_group("_HOSTILE_JESUS");

		Ped ped = ped::spawn_in_vehicle("u_m_m_jesus_01", veh, true);

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		PED::SET_PED_HEARING_RANGE(ped, 9999.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true);

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		TASK::TASK_PLANE_MISSION(ped, veh, 0, target, 0, 0, 0, 6, 0.0, 0.0, 0.0, 2500.0, -1500.0, 0);

		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		TASK::TASK_COMBAT_PED(ped, target, 0, 16);

		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));

		return ped;
	}

	///
	/// GPL-3 CODE ENDS HERE
	///

	inline void set_ped_random_component_variation(Ped ped)
	{
		auto range = [](int lower_bound, int upper_bound) -> int {
			return std::rand() % (upper_bound - lower_bound + 1) + lower_bound;
		};
		outfit::components_t components;
		for (auto& item : components.items)
		{
			int drawable_id = range(0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(ped, item.id) - 1);
			int texture_id  = range(0, PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(ped, item.id, drawable_id) - 1);
			PED::SET_PED_COMPONENT_VARIATION(ped, item.id, drawable_id, texture_id, PED::GET_PED_PALETTE_VARIATION(ped, item.id));
		}
	}
}
