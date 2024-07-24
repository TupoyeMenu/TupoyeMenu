#include "backend/bool_command.hpp"
#include "backend/command.hpp"
#include "natives.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "util/string_operations.hpp"
#include "util/vehicle.hpp"
#include "util/entity.hpp"

namespace big
{
	class spawn_vehicle : command
	{
		using command::command;

		virtual std::optional<std::vector<std::string>> get_argument_suggestions(int arg) override
		{
			if (arg == 1)
			{
				std::vector<std::string> suggestions;
				for (auto& item : g_gta_data_service->vehicles())
				{
					suggestions.push_back(item.second.m_name);
				}
				return suggestions;
			}

			return std::nullopt;
		}

		virtual std::optional<command_arguments> parse_args(const std::vector<std::string>& args, const std::shared_ptr<command_context> ctx) override
		{
			command_arguments result(1);

			if (g_gta_data_service->vehicle_by_hash(rage::joaat(args[0])).m_hash != 0)
			{
			result.push(rage::joaat(args[0]));
				return result;
			}

			for (auto& item : g_gta_data_service->vehicles())
			{
				std::string item_name_lower, args_lower;
				item_name_lower = item.second.m_name;
				args_lower      = args[0];
				string::operations::to_lower(item_name_lower);
				string::operations::to_lower(args_lower);
				if (item_name_lower.find(args_lower) != std::string::npos)
				{
					result.push(rage::joaat(item.first));
					return result;
				}
			}

			return result;
		}

		virtual CommandAccessLevel get_access_level() override
		{
			return CommandAccessLevel::FRIENDLY;
		}

		virtual void execute(const command_arguments& args, const std::shared_ptr<command_context> ctx) override
		{
			const auto hash = args.get<rage::joaat_t>(0);
			if (!entity::request_model(hash))
			{
				ctx->report_error("Specified model is invalid.");
				return;
			}

			auto id = ctx->get_sender()->id();

			const auto spawn_location = vehicle::get_spawn_location(id == self::id ? g.spawn_vehicle.spawn_inside : false,
			        hash, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(id));
			const auto spawn_heading = ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(id));

			auto veh = vehicle::spawn(hash, spawn_location, spawn_heading);

			if (veh == 0)
			{
				g_notification_service.push_error("Spawn Vehicle", "Unable to spawn vehicle");
			}
			else
			{
				if (g.spawn_vehicle.spawn_maxed)
				{
					vehicle::max_vehicle(veh);
				}

				if (g.spawn_vehicle.spawn_inside && ctx->get_sender()->id() == self::id)
				{
					vehicle::teleport_into_vehicle(veh);
				}
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
			}
		}
	};

	spawn_vehicle g_spawn_vehicle("spawn", "Spawn Vehicle", "Spawn a vehicle with the specified model.", 1);
	bool_command g_spawn_maxed("spawnmaxed", "Spawn Maxed", "Controls whether the vehicle spawned will have its mods maxed out", g.spawn_vehicle.spawn_maxed);
	bool_command g_spawn_inside("spawnin", "Spawn Inside", "Controls whether the player should be set inside the vehicle after it spawns", g.spawn_vehicle.spawn_inside);
}
