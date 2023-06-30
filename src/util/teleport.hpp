#pragma once
#include "blip.hpp"
#include "entity.hpp"
#include "gta/enums.hpp"
#include "gta/net_object_mgr.hpp"
#include "services/players/player_service.hpp"
#include "vehicle.hpp"

namespace big::teleport
{
	struct telelocation
	{
		std::string name;
		float x, y, z;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(telelocation, name, x, y, z);

	inline std::map<std::string, std::vector<telelocation>> all_saved_locations;

	inline std::filesystem::path get_telelocations_file_path()
	{
		return g_file_manager->get_project_file("telelocations.json").get_path();
	}

	inline bool fetch_saved_locations()
	{
		all_saved_locations.clear();

		auto path = get_telelocations_file_path();
		std::ifstream file(path, std::ios::binary);

		try
		{
			if (!file.is_open())
				return false;

			nlohmann::json j;
			file >> j;
			all_saved_locations = j.get<std::map<std::string, std::vector<telelocation>>>();

			return true;
		}
		catch (const std::exception& e)
		{
			LOG(WARNING) << "Failed fetching saved locations: " << e.what() << '\n';
			return false;
		}

		return false;
	}

	inline bool save_new_location(const std::string& category, telelocation t)
	{
		const auto& pair = all_saved_locations.insert({category, {t}});
		if (!pair.second)
		{
			pair.first->second.push_back(t);
		}

		auto path = get_telelocations_file_path();

		std::ofstream file_out(path, std::ofstream::trunc | std::ofstream::binary);
		if (!file_out.is_open())
			return false;

		nlohmann::json j = all_saved_locations;
		file_out << j.dump(4);
		file_out.close();

		return true;
	}

	inline bool delete_saved_location(const std::string& category, const std::string& location_name)
	{
		auto path = get_telelocations_file_path();

		const auto& it = all_saved_locations.find(category);
		if (it == all_saved_locations.end())
			return false;

		std::erase_if(it->second, [location_name](telelocation t) {
			return t.name == location_name;
		});

		if (!it->second.size())
		{
			all_saved_locations.erase(category);
		}

		std::ofstream file_out(path, std::ofstream::trunc | std::ofstream::binary);
		if (!file_out.is_open())
			return false;

		nlohmann::json j = all_saved_locations;
		file_out << j.dump(4);
		file_out.close();

		return true;
	}

	inline bool teleport_player_to_coords(player_ptr player, Vector3 coords)
	{
		Entity ent;

		if (*g_pointers->m_gta.m_is_session_started)
			ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id());
		else
			ent = PLAYER::PLAYER_PED_ID();

		if (ent == self::ped || ent == self::veh)
			PED::SET_PED_COORDS_KEEP_VEHICLE(ent, coords.x, coords.y, coords.z);

		if (ENTITY::IS_ENTITY_DEAD(ent, true))
		{
			g_notification_service->push_warning("TELEPORT"_T.data(), "TELEPORT_PLAYER_IS_DEAD"_T.data());
			return false;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(ent, true))
		{
			ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

			if (entity::take_control_of(ent))
				ENTITY::SET_ENTITY_COORDS(ent, coords.x, coords.y, coords.z, 0, 0, 0, 0);
			else
				g_notification_service->push_warning("TELEPORT"_T.data(), "TELEPORT_FAILED_TO_TAKE_CONTROL"_T.data());

			return true;
		}
		else
		{
			auto hnd = vehicle::spawn(RAGE_JOAAT("ninef"), *player->get_ped()->get_position(), 0.0f, true);
			ENTITY::SET_ENTITY_VISIBLE(hnd, false, false);
			ENTITY::SET_ENTITY_COLLISION(hnd, false, false);
			ENTITY::FREEZE_ENTITY_POSITION(hnd, true);

			auto obj_id                      = player->get_ped()->m_net_object->m_object_id;
			remote_player_teleport remote_tp = {obj_id, {coords.x, coords.y, coords.z}};

			g.m_remote_player_teleports.emplace(g_pointers->m_gta.m_handle_to_ptr(hnd)->m_net_object->m_object_id, remote_tp);

			if ((player->is_valid() && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->id()), false))
			    || PLAYER::IS_REMOTE_PLAYER_IN_NON_CLONED_VEHICLE(player->id()))
				g_pointers->m_gta.m_clear_ped_tasks_network(player->get_ped(), true);

			for (int i = 0; i < 15; i++)
			{
				script::get_current()->yield(50ms);

				if (auto ptr = (rage::CDynamicEntity*)g_pointers->m_gta.m_handle_to_ptr(hnd))
				{
					if (auto netobj = ptr->m_net_object)
					{
						g_pointers->m_gta.m_migrate_object(player->get_net_game_player(), netobj, 3);
					}
				}
			}

			entity::delete_entity(hnd);

			std::erase_if(g.m_remote_player_teleports, [obj_id](auto& obj) {
				return obj.first == obj_id;
			});

			return true;
		}
	}

	inline bool godmode_kill(player_ptr player)
	{
		return teleport_player_to_coords(player, Vector3(883.5, -986.5, 31.3));
	}

	inline bool bring_player(player_ptr player)
	{
		return teleport_player_to_coords(player, self::pos);
	}

	inline bool into_vehicle(Vehicle veh)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh))
		{
			g_notification_service->push_warning("TELEPORT"_T.data(), "TELEPORT_INVALID_VEHICLE"_T.data());

			return false;
		}

		int seat_index = 255;
		if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1, true))
			seat_index = -1;
		else if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -2, true))
			seat_index = -2;

		if (seat_index == 255)
		{
			g_notification_service->push_warning("TELEPORT"_T.data(), "TELEPORT_NO_SEATS_FREE"_T.data());

			return false;
		}

		Vector3 location = ENTITY::GET_ENTITY_COORDS(veh, true);
		entity::load_ground_at_3dcoord(location);

		Ped ped = self::ped;

		ENTITY::SET_ENTITY_COORDS(ped, location.x, location.y, location.z, 0, 0, 0, 0);

		script::get_current()->yield();

		PED::SET_PED_INTO_VEHICLE(ped, veh, seat_index);

		return true;
	}

	inline void to_coords(Vector3 location)
	{
		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z + 1.f);
	}

	inline bool to_blip(int sprite, int color = -1)
	{
		Vector3 location;

		if (!blip::get_blip_location(location, sprite, color))
			return false;

		if (sprite == (int)BlipIcons::Waypoint)
			entity::load_ground_at_3dcoord(location);

		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z);

		return true;
	}

	inline bool to_entity(Entity ent)
	{
		Vector3 location = ENTITY::GET_ENTITY_COORDS(ent, true);

		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z);

		return true;
	}

	inline bool to_player(Player player)
	{
		return to_entity(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
	}

	inline bool to_waypoint()
	{
		if (!to_blip((int)BlipIcons::Waypoint))
		{
			g_notification_service->push_warning("TELEPORT"_T.data(), "TELEPORT_NO_WAYPOINT_SET"_T.data());

			return false;
		}
		return true;
	}

	inline bool to_objective()
	{
		Vector3 location;

		if (!blip::get_objective_location(location))
		{
			g_notification_service->push_warning("TELEPORT"_T.data(), "TELEPORT_NO_OBJECTIVE"_T.data());
			return false;
		}

		PED::SET_PED_COORDS_KEEP_VEHICLE(self::ped, location.x, location.y, location.z);

		return false;
	}

	inline bool tp_on_top(Entity ent, bool match_velocity)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(ent))
			return false;

		Vector3 ent_dimensions_max{}, ent_dimensions_min{}, ent_pos{};

		MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(ent), &ent_dimensions_min, &ent_dimensions_max);
		ent_pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, 0, 0, ent_dimensions_max.z);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(self::ped, ent_pos.x, ent_pos.y, ent_pos.z, 0, 0, 0);

		if (match_velocity)
		{
			auto ent_velocity = ENTITY::GET_ENTITY_VELOCITY(ent);
			ENTITY::SET_ENTITY_VELOCITY(self::ped, ent_velocity.x, ent_velocity.y, ent_velocity.z);
		}

		return false;
	}
}
