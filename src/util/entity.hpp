/**
 * @file entity.hpp
 * @brief Basic entity related functions.
 */

#pragma once
#include <script/types.hpp>
#include <rage/vector.hpp>

namespace rage
{
	class netObject;
	class CEntity;
}

namespace big::entity
{
	/**
	 * @brief Attemts to take control of given entity
	 * 
	 * @param ent Entity to take control of.
	 * @param timeout When to give up trying to take control. In for loop iterations. 
	 * @return True if the control has been taken.
	 */
	bool take_control_of(Entity ent, int timeout = 300);

	/**
	 * @brief Deletes given entity.
	 *
	 * @note Attempts to take control of entity on it's own.
	 * 
	 * @param ent Entity to delete
	 */
	void delete_entity(Entity& ent, bool force = false);

	/**
	 * @brief Raycasts for entitys from the current camera forward.
	 * 
	 * @param ent Entity that we have hit.
	 * @return True if hit something other then the sky.
	 */
	bool raycast(Entity* ent);
	bool raycast(Vector3* endcoor);

	/**
	 * @brief Checks if you have control over net_object
	 * 
	 * @param net_object Entity to check control of.
	 * @return True if you have control over entity.
	 */
	bool network_has_control_of_entity(rage::netObject* net_object);

	/**
	 * @brief Gets all entitys from game pools.
	 * 
	 * @note Does not include local player or local vehicle.
	 * 
	 * @param vehicles Include vehicles.
	 * @param peds Include peds.
	 * @param props Include props. Default: false.
	 * @param include_self_veh Include vehicle local player is in. Default: false.
	 * @return std::vector<Entity> of all entitys in game pools.
	 */
	std::vector<Entity> get_entities(bool vehicles, bool peds, bool props = false, bool include_self_veh = false);
	bool load_ground_at_3dcoord(Vector3& location);
	bool request_model(rage::joaat_t hash);
	double distance_to_middle_of_screen(const rage::fvector2& screen_pos);
	Entity get_entity_closest_to_middle_of_screen(rage::fwEntity** pointer = nullptr, std::vector<Entity> ignore_entities = {}, bool include_veh = true, bool include_ped = true, bool include_prop = true, bool include_players = true);
	void force_remove_network_entity(rage::CDynamicEntity* entity, bool delete_locally = true);
	void force_remove_network_entity(std::uint16_t net_id, int ownership_token = -1, bool delete_locally = true);
}
