/**
 * @file ped.hpp
 * @brief Basic ped related functions.
 */

#pragma once
#include "gta/enums.hpp"
#include "services/players/player_service.hpp"

namespace big::ped
{
	/**
	 * @brief Spawns "prop_gold_cont_01" inside the ped.
	 * 
	 * @param ped Ped to cage.
	 */
	void cage_ped(Ped ped);

	/**
	 * @brief Removes decals from ped.
	 * 
	 * @param ped Ped to remove dacals from.
	 */
	void clean_ped(Ped ped);

	/**
	 * @brief Sets the model of local ped.
	 * 
	 * @param hash Model hash.
	 * @return True model has been set successfully.
	 */
	bool change_player_model(const Hash hash);

	/**
	 * @brief Copys outfit from target to local ped.
	 * 
	 * @param target Target to copy from.
	 * @return True if models are the same.
	 */
	bool steal_outfit(const Ped target);

	void clone_ped(const Ped src, const Ped target);

	/**
	 * @brief Sets your local ped to be the same as target ped.
	 * 
	 * @param target Ped to copy from.
	 */
	void steal_identity(const Ped target);

	/**
	 * @brief Takes control of ped and applies sets it's health to 0.
	 * If failed to take control sends a network damage event.
	 * 
	 * @param ped Ped to apply damage to.
	 */
	void kill_ped(const Ped ped);

	/**
	 * @brief Kills the ped if it's ralationship with player equals `relation_id`.
	 * 
	 * @param ped Ped to kill.
	 * @param relation_id Relation ID to check.
	 */
	void kill_ped_by_relation(Ped ped, int relation_id);

	/**
	 * @brief Spawns ped in vehicle.
	 * 
	 * @param hash Model hash to spawn.
	 * @param veh Vehicle to spawn in.
	 * @param is_networked Is this ped synced to other players.
	 * @return Spawned ped.
	 */
	Ped spawn_in_vehicle(Hash hash, Vehicle veh, bool is_networked = true);

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
	Ped spawn(ePedType pedType, Hash hash, Ped clone, Vector3 location, float heading, bool is_networked = true);

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
	void set_attacker_ped_flags(Ped ped, Ped ped_to_attack);


	/**
	 * @brief Creates a Jesus ped with railgun that attacks everyone.
	 * 
	 * @param target The main target for jesus.
	 * @return Created ped.
	 */
	Ped spawn_griefer_jesus(Ped ped_to_attack);

	/**
	 * @brief Creates a Jesus ped with railgun on Oppressor Mk2 that attacks everyone.
	 * 
	 * @param ped_to_attack The main target for jesus.
	 * @return Created ped.
	 */
	Ped spawn_extrime_griefer_jesus(Ped ped_to_attack);

	/**
	 * @brief Creates a Jesus ped with railgun in a vehicle that attacks everyone.
	 * 
	 * @param ped_to_attack The main target for the jet.
	 * @param vehicle Hash of the vehicle you want ped to spawn in.
	 * @return Created ped.
	 */
	Ped spawn_griefer_jet(Ped ped_to_attack, Hash vehicle);

	/**
	 * @brief Randomizes ped clothing.
	 * 
	 * @param ped Ped to randomize clothing of.
	 */
	void set_ped_random_component_variation(Ped ped);

	/**
	 * @brief Gets player_ptr from player ped.
	 * 
	 * @param ped Ped to search.
	 * @return player_ptr of found player, if player was not found nullptr.
	 */
	player_ptr get_player_from_ped(Ped ped);

	/**
	 * @brief Loads animation dictionary.
	 * 
	 * @param dict Dictionary name.
	 * @return True if animation_dict has loaded.
	 */
	bool load_animation_dict(const char* dict);

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
	void ped_play_animation(Ped ped, const std::string_view& animDict, const std::string_view& animName, float speed = 4.f, float speedMultiplier = -4.f, int duration = -1, int flag = 0, float playbackRate = 0, bool lockPos = false, Vector3 pos = {}, Vector3 rot = {}, int ik_flags = 0);

	/**
	* @brief Will make the ped enter the vehicle with animation if vehicle is in vicinity
	*
	* @param ped Ped to use.
	* @param veh Vehicle to enter.
	* @param seat Seat to enter.
	* @param movespeed 1 = walk, 2 = run, 3 = sprint
	*/
	void ped_enter_vehicle_animated(Ped ped, Vehicle veh, eVehicleSeats seat, int movespeed);

	bool is_ped_a_friend(Ped ped, CPed* ped_ptr);
}
