/**
 * @file ped.hpp
 * @brief Basic ped related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "entity.hpp"
#include "gta/enums.hpp"
#include "math.hpp"
#include "natives.hpp"
#include "outfit.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "services/players/player_service.hpp"
#include "vehicle.hpp"

namespace big::ped
{
	/**
	 * @brief Spawns "prop_gold_cont_01" inside the ped.
	 * 
	 * @param ped Ped to cage.
	 */
	inline void cage_ped(Ped ped)
	{
		Hash hash = RAGE_JOAAT("prop_gold_cont_01");

		Vector3 location = ENTITY::GET_ENTITY_COORDS(ped, true);
		OBJECT::CREATE_OBJECT(hash, location.x, location.y, location.z - 1.f, true, false, false);
	}

	/**
	 * @brief Removes decals from ped.
	 * 
	 * @param ped Ped to remove dacals from.
	 */
	inline void clean_ped(Ped ped)
	{
		Ped player_ped = self::ped;

		PED::CLEAR_PED_BLOOD_DAMAGE(player_ped);
		PED::CLEAR_PED_WETNESS(player_ped);
		PED::CLEAR_PED_ENV_DIRT(player_ped);
		PED::RESET_PED_VISIBLE_DAMAGE(player_ped);
	}

	/**
	 * @brief Sets the model of local ped.
	 * 
	 * @param hash Model hash.
	 * @return True model has been set successfully.
	 */
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

	/**
	 * @brief Copys outfit from target to local ped.
	 * 
	 * @param target Target to copy from.
	 * @return True if models are the same.
	 */
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

	/**
	 * @brief Sets your local ped to be the same as target ped.
	 * 
	 * @param target Ped to copy from.
	 */
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

	/**
	 * @brief Takes control of ped and applys max damage to it.
	 * 
	 * @param ped Ped to apply damage to.
	 */
	inline void kill_ped(const Ped ped)
	{
		if (entity::take_control_of(ped, 0))
			ENTITY::SET_ENTITY_HEALTH(ped, 0, self::ped);
		else
		{
			auto ptr = g_pointers->m_gta.m_handle_to_ptr(ped);
			if (!ptr)
				return;
			g_pointers->m_gta.m_send_network_damage(g_player_service->get_self()->get_ped(), ptr, ptr->get_position(), 0, true, RAGE_JOAAT("weapon_explosion"), 10000.0f, 2, 0, (1 << 4), 0, 0, 0, false, false, true, true, nullptr);
		}
	}

	/**
	 * @brief Kills the ped if it's ralationship with player equals `relation_id`.
	 * 
	 * @param ped Ped to kill.
	 * @param relation_id Relation ID to check.
	 */
	inline void kill_ped_by_relation(Ped ped, int relation_id)
	{
		if (PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, PLAYER::PLAYER_PED_ID()) == relation_id)
			kill_ped(ped);
	}

	/**
	 * @brief Spawns ped in vehicle.
	 * 
	 * @param hash Model hash to spawn.
	 * @param veh Vehicle to spawn in.
	 * @param is_networked Is this ped synced to other players.
	 * @return Spawned ped.
	 */
	inline Ped spawn_in_vehicle(Hash hash, Vehicle veh, bool is_networked = true)
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

		return ped;
	}

	/**
	 * @brief Spawns ped.
	 * 
	 * @param pedType Ped type to use.
	 * @param hash Model hash to spawn.
	 * @param clone Ped to clone from, using PED::CLONE_PED_TO_TARGET(Ped, Ped), use 0 to ignore.
	 * @param location Location to spawn ped on.
	 * @param heading The direction ped is facing.
	 * @param is_networked Is this ped synced to other players.
	 * @return Spawned ped.
	 */
	inline Ped spawn(ePedType pedType, Hash hash, Ped clone, Vector3 location, float heading, bool is_networked = true)
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

	/**
	 * @brief Makes the ped attack the target.
	 * - Adds this ped to `HATES_PLAYER` relationship group.
	 * - Sets hearing range to 5000.
	 * - Disables dying animation.
	 * - Prevents ragdoll from player impact, bullets and fire.
	 * - Attacks the target ped.
	 * 
	 * @param ped Ped to apply flags to.
	 * @param ped_to_attack Ped to attack
	 */
	inline void set_attacker_ped_flags(Ped ped, Ped ped_to_attack)
	{
		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, RAGE_JOAAT("HATES_PLAYER"));
		PED::SET_PED_HEARING_RANGE(ped, 5000.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true); // Disables the dumb scripted dying animation.

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true); // Always fight.

		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);          // Don't ragoll from fire and bullets.
		TASK::TASK_COMBAT_PED(ped, ped_to_attack, 0, 16); // 16 allows to fight armed peds.
	}

	/**
	 * @brief Creates a Jesus ped with railgun that attacks everyone.
	 * 
	 * @param target The main target of the ped.
	 * @return Created ped.
	 */
	inline Ped spawn_griefer_jesus(Ped ped_to_attack)
	{
		Ped ped;
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped_to_attack, false) + Vector3(0, 0, 1);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped_to_attack, false))
			ped = ped::spawn_in_vehicle(ePedType::PED_TYPE_CRIMINAL, RAGE_JOAAT("u_m_m_jesus_01"), true);
		else
			ped = ped::spawn(ePedType::PED_TYPE_CRIMINAL, RAGE_JOAAT("u_m_m_jesus_01"), 0, pos, 0);

		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);
		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));
		ped::set_attacker_ped_flags(ped, ped_to_attack);

		return ped;
	}

	/**
	 * @brief Creates a Jesus ped with railgun on Oppressor Mk2 that attacks everyone.
	 * 
	 * @param ped_to_attack The main target of the ped.
	 * @return Created ped.
	 */
	inline Ped spawn_extrime_griefer_jesus(Ped ped_to_attack)
	{
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped_to_attack, false) + Vector3(0, 0, 5);
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(ped_to_attack, false) ? PED::GET_VEHICLE_PED_IS_IN(ped_to_attack, false) : ped_to_attack);

		Vehicle veh = vehicle::spawn(RAGE_JOAAT("oppressor2"), pos, heading, true, false, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		vehicle::max_vehicle(veh);
		ENTITY::SET_ENTITY_PROOFS(veh, false, true, true, false, false, false, false, false); // Fire and explosion proof.

		Ped ped = ped::spawn_in_vehicle(RAGE_JOAAT("u_m_m_jesus_01"), veh, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);   // Never leave the vehicle.
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1); // Disable knock off vehicle.
		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false); // Fire and explosion proof.
		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));
		ped::set_attacker_ped_flags(ped, ped_to_attack);

		return ped;
	}

	/**
	 * @brief Creates a Jesus ped with railgun in a vehicle that attacks everyone.
	 * 
	 * @param ped_to_attack The main target of the ped.
	 * @param vehicle Hash of the vehicle you want ped to spawn in.
	 * @return Created ped.
	 */
	inline Ped spawn_griefer_jet(Ped ped_to_attack, Hash vehicle)
	{
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped_to_attack, false) + Vector3(0, 0, 30);
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(ped_to_attack, false) ? PED::GET_VEHICLE_PED_IS_IN(ped_to_attack, false) : ped_to_attack);

		Vehicle veh = vehicle::spawn(vehicle, pos, heading, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		VEHICLE::CONTROL_LANDING_GEAR(veh, 3);

		Ped ped = ped::spawn_in_vehicle(RAGE_JOAAT("u_m_m_jesus_01"), veh, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		TASK::TASK_PLANE_MISSION(ped, veh, PED::GET_VEHICLE_PED_IS_IN(ped_to_attack, false), ped_to_attack, 0, 0, 0, 6, 150.f, 0.f, 0.f, 2500.f, -200.f, -1.f);
		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));
		ped::set_attacker_ped_flags(ped, ped_to_attack);

		return ped;
	}

	/**
	 * @brief Randomizes ped clothing.
	 * 
	 * @param ped Ped to randomize clothing of.
	 */
	inline void set_ped_random_component_variation(Ped ped)
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

	/**
	 * @brief Gets player_ptr from player ped.
	 * 
	 * @param ped Ped to search.
	 * @return player_ptr of found player, if player was not found nullptr.
	 */
	inline player_ptr get_player_from_ped(Ped ped)
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

	/**
	 * @brief Loads animation dictionary.
	 * 
	 * @param dict Dictionary name.
	 * @return True if animation_dict has loaded.
	 */
	inline bool load_animation_dict(const char* dict)
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

	/**
	 * @brief Plays an animation on a ped.
	 * 
	 * @param ped Ped to play animation on.
	 * @param animDict Animation dictionary name.
	 * @param animName Animation name.
	 * @param speed Blend in speed.
	 * @param speedMultiplier Blend out speed.
	 * @param duration The ammount of time this animation can play, use -1 for default.
	 * @param flag You know were to look for enum.
	 */
	inline void ped_play_animation(Ped ped, const std::string_view& animDict, const std::string_view& animName, float speed = 4.f, float speedMultiplier = -4.f, int duration = -1, int flag = 0, float playbackRate = 0, bool lockPos = false, Vector3 pos = {}, Vector3 rot = {}, int ik_flags = 0)
	{
		if (load_animation_dict(animDict.data()))
		{
			if (pos.x == 0 && pos.y == 0 && pos.z == 0)
				TASK::TASK_PLAY_ANIM(ped, animDict.data(), animName.data(), speed, speedMultiplier, duration, flag, playbackRate, lockPos, lockPos, lockPos);
			else
				TASK::TASK_PLAY_ANIM_ADVANCED(ped, animDict.data(), animName.data(), pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, speed, speedMultiplier, duration, flag, playbackRate, lockPos, ik_flags);
		}
	}

	/**
	* @brief Will make the ped enter the vehicle with animation if vehicle is in vicinity
	*
	* @param ped Ped to use.
	* @param veh Vehicle to enter.
	* @param seat Seat to enter.
	* @param movespeed 1 = walk, 2 = run, 3 = sprint
	*/
	inline void ped_enter_vehicle_animated(Ped ped, Vehicle veh, eVehicleSeats seat, int movespeed)
	{
		if (entity::take_control_of(ped))
		{
			if (ENTITY::DOES_ENTITY_EXIST(veh))
			{
				if (math::distance_between_vectors(ENTITY::GET_ENTITY_COORDS(ped, 0), ENTITY::GET_ENTITY_COORDS(veh, 0)) < 15.f)
					TASK::TASK_ENTER_VEHICLE(ped, veh, 10000, (int)seat, movespeed, 8, NULL);
				else
					PED::SET_PED_INTO_VEHICLE(ped, veh, (int)seat);
			}
		}
	}

}
