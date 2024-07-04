/**
 * @file view_player_info.cpp
 * @brief Information about selected player.
 */

#include "core/data/block_join_reasons.hpp"
#include "core/data/command_access_levels.hpp"
#include "core/data/game_states.hpp"
#include "core/data/language_codes.hpp"
#include "core/scr_globals.hpp"
#include "gui/components/components.hpp"
#include "natives.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "services/player_database/player_database_service.hpp"
#include "util/math.hpp"
#include "util/misc.hpp"
#include "views/view.hpp"

#include <network/netConnection.hpp>
#include <script/globals/GPBD_FM.hpp>
#include <script/globals/GPBD_FM_3.hpp>
#include <script/globals/GlobalPlayerBD.hpp>

namespace big
{
	const char* get_nat_type_str(int type)
	{
		switch (type)
		{
		case 1: return "Open";
		case 2: return "Moderate";
		case 3: return "Strict";
		}

		return "Unknown";
	}

	const char* get_connection_type_str(int type)
	{
		switch (type)
		{
		case 1: return "Direct";
		case 2: return "Relay";
		case 3: return "Peer Relay";
		}

		return "Unknown";
	}

	void add_damage_flag(std::string& mode_str, const std::string& name)
	{
		if (!mode_str.empty())
			mode_str += ", ";
		mode_str += name;
	}

	void view::view_player_info()
	{
		std::string title        = std::format("Player Info: {}", g_player_service->get_selected()->get_name());
		CPlayerInfo* player_info = g_player_service->get_selected()->get_player_info();
		CPed* cped               = g_player_service->get_selected()->get_ped();

		ImGui::TextUnformatted(title.c_str());
		ImGui::Checkbox("Spectate", &g.player.spectating);

		ImGui::Text("Player ID: %d", g_player_service->get_selected()->id());

		ImGui::Text("Session Host: %s", g_player_service->get_selected()->is_host() ? "Yes" : "No");

		if (ImGui::Button("Add To Database"))
		{
			g_player_database_service->get_or_create_player(g_player_service->get_selected());
		}

		ImGui::Separator();

		if (player_info != nullptr)
		{
			ImGui::Text("Host token: %llu", player_info->m_net_player_data.m_host_token);
			if (ImGui::Button("Copy##HostToken"))
				{
					ImGui::SetClipboardText(std::format("{:X}", g_player_service->get_selected()->get_net_data()->m_host_token).data());
				}
			ImGui::Text("Wanted Level: %d", player_info->m_wanted_level);
		}

		uint32_t ped_damage_bits = 0;
		uint32_t ped_task_flag   = 0;
		uint32_t veh_damage_bits = 0;
		float ped_health         = 0.f;
		float ped_max_health     = 0.f;
		std::string mode_str     = "";

		if (CPed* ped = g_player_service->get_selected()->get_ped())
		{
			ped_damage_bits = ped->m_damage_bits;
			ped_task_flag   = ped->m_ped_task_flag;
			ped_health   = ped->m_health;
			ped_max_health   = ped->m_maxhealth;
		}

		if (ped_damage_bits & (uint32_t)eEntityProofs::GOD)
		{
			mode_str = "God";
		}
		else
		{
			if (ped_damage_bits & (uint32_t)eEntityProofs::BULLET)
			{
				add_damage_flag(mode_str, "Bullet");
			}
			if (ped_damage_bits & (uint32_t)eEntityProofs::FIRE)
			{
				add_damage_flag(mode_str, "Fire");
			}
			if (ped_damage_bits & (uint32_t)eEntityProofs::COLLISION)
			{
				add_damage_flag(mode_str, "Collision");
			}
			if (ped_damage_bits & (uint32_t)eEntityProofs::MELEE)
			{
				add_damage_flag(mode_str, "Melee");
			}
			if (ped_damage_bits & (uint32_t)eEntityProofs::EXPLOSION)
			{
				add_damage_flag(mode_str, "Explosion");
			}
			if (ped_damage_bits & (uint32_t)eEntityProofs::STEAM)
			{
				add_damage_flag(mode_str, "Steam");
			}
			if (ped_damage_bits & (uint32_t)eEntityProofs::WATER)
			{
				add_damage_flag(mode_str, "Water");
			}
			if(ped_health > 328 || ped_max_health > 328)
			{
				add_damage_flag(mode_str, "Health Too High");
			}
		}

		if (mode_str.empty())
		{
			mode_str = "No";
		}

		ImGui::Text("Player proofs: %s", mode_str.c_str());

		mode_str = "";

		if (CVehicle* vehicle = g_player_service->get_selected()->get_current_vehicle(); vehicle != nullptr)
		{
			veh_damage_bits = vehicle->m_damage_bits;
		}

		if (ped_task_flag & (uint8_t)ePedTask::TASK_DRIVING)
		{
			if (veh_damage_bits & (uint32_t)eEntityProofs::GOD)
			{
				mode_str = "God";
			}
			else
			{
				if (veh_damage_bits & (uint32_t)eEntityProofs::BULLET)
				{
					add_damage_flag(mode_str, "Bullet");
				}
				if (veh_damage_bits & (uint32_t)eEntityProofs::FIRE)
				{
					add_damage_flag(mode_str, "Fire");
				}
				if (veh_damage_bits & (uint32_t)eEntityProofs::COLLISION)
				{
					add_damage_flag(mode_str, "Collision");
				}
				if (veh_damage_bits & (uint32_t)eEntityProofs::MELEE)
				{
					add_damage_flag(mode_str, "Melee");
				}
				if (veh_damage_bits & (uint32_t)eEntityProofs::EXPLOSION)
				{
					add_damage_flag(mode_str, "Explosion");
				}
				if (veh_damage_bits & (uint32_t)eEntityProofs::STEAM)
				{
					add_damage_flag(mode_str, "Steam");
				}
				if (veh_damage_bits & (uint32_t)eEntityProofs::WATER)
				{
					add_damage_flag(mode_str, "Water");
				}
			}

			if (mode_str.empty())
			{
				mode_str = "No";
			}
		}
		else
		{
			mode_str = "No vehicle detected";
		}

		ImGui::Text("Vehicle proofs: %s", mode_str.c_str());

		if (auto net_player_data = g_player_service->get_selected()->get_net_data())
		{
			if (ImGui::TreeNode("Net Info"))
			{
				ImGui::Text("Rockstar ID: %llu", net_player_data->m_gamer_handle.m_rockstar_id);

				ImGui::SameLine();

				if (ImGui::Button("Copy##RID"))
					ImGui::SetClipboardText(std::to_string(net_player_data->m_gamer_handle.m_rockstar_id).data());

				auto ip   = g_player_service->get_selected()->get_ip_address();
				auto port = g_player_service->get_selected()->get_port();

				if (ip)
				{
					ImGui::Text("IP Address: %d.%d.%d.%d:%d",
					    ip.value().m_field1,
					    ip.value().m_field2,
					    ip.value().m_field3,
					    ip.value().m_field4,
					    port);

					ImGui::SameLine();

					// clang-format off
					ImGui::PushID("##ip");
					if (ImGui::SmallButton("Copy"))
						ImGui::SetClipboardText(std::format("{}.{}.{}.{}:{}",
						    ip.value().m_field1,
						    ip.value().m_field2,
						    ip.value().m_field3,
						    ip.value().m_field4,
						    port).data());
					ImGui::PopID();
					// clang-format on
				}
				else
				{
					if (net_player_data->m_force_relays) // TODO: does this actually do anything?
						ImGui::TextUnformatted("IP Address: Hidden");
					else
						ImGui::TextUnformatted("IP Address: Unknown");

					auto cxn_type = g_player_service->get_selected()->get_connection_peer() ?
					    g_player_service->get_selected()->get_connection_peer()->m_peer_address.m_connection_type :
					    0;

					ImGui::SameLine();

					if (g.protections.force_relay_connections)
						components::help_marker("IP addresses cannot be seen when Force Relay Connections is enabled");
					else if (cxn_type == 2)
						components::help_marker("Cannot retrieve IP address since this player is connected through dedicated servers");
					else if (cxn_type == 3)
						components::help_marker("Cannot retrieve IP address since this player is connected through another player");
				}

				ImGui::Text("Game State: %s", game_states[(int32_t)player_info->m_game_state]);

				ImGui::Text("NAT Type: %s", get_nat_type_str(g_player_service->get_selected()->get_net_data()->m_nat_type));

				if (auto peer = g_player_service->get_selected()->get_connection_peer())
				{
					ImGui::Text("Connection Type: %s", get_connection_type_str(peer->m_peer_address.m_connection_type));

					if (peer->m_peer_address.m_connection_type == 2)
					{
						auto ip = peer->m_relay_address.m_relay_address;
						ImGui::Text("Relay IP: %d.%d.%d.%d", ip.m_field1, ip.m_field2, ip.m_field3, ip.m_field4);
					}
					else if (peer->m_peer_address.m_connection_type == 3)
					{
						auto ip = peer->m_peer_address.m_relay_address;
						ImGui::Text("Peer Relay IP: %d.%d.%d.%d", ip.m_field1, ip.m_field2, ip.m_field3, ip.m_field4);
					}

					ImGui::Text("Num Messages Sent: %d", peer->m_num_messages_batched);
					ImGui::Text("Num Reliables Sent: %d", peer->m_num_reliable_messages_batched);
					ImGui::Text("Num Reliables Resent: %d", peer->m_num_resent_reliable_messages_batched);
					ImGui::Text("Num Encryption Attempts: %d", peer->m_num_encryption_attempts);
				}

				components::button("Open SC Overlay", [] {
					int gamerHandle;
					NETWORK::NETWORK_HANDLE_FROM_PLAYER(g_player_service->get_selected()->id(), &gamerHandle, 13);
					NETWORK::NETWORK_SHOW_PROFILE_UI(&gamerHandle);
				});
				ImGui::TreePop();
			}

			if (auto current_player = g_player_database_service->get_player_by_rockstar_id(
			        g_player_service->get_selected()->get_net_data()->m_gamer_handle.m_rockstar_id))
			{
				if (ImGui::TreeNode("Player DB Info"))
				{
					if (!current_player->infractions.empty())
					{
						ImGui::TextUnformatted("Infractions:");
						for (auto& infraction : current_player->infractions)
						{
							ImGui::BulletText("%s", current_player->get_infraction_description(infraction));
						}
					}

					if (ImGui::Checkbox("Is Modder", &current_player->is_modder)
					    || ImGui::Checkbox("Force Allow Join", &current_player->force_allow_join)
					    || ImGui::Checkbox("Block Join", &current_player->block_join))
					{
						g_player_service->get_selected()->is_modder  = current_player->is_modder;
						g_player_service->get_selected()->block_join = current_player->block_join;
						g_player_database_service->save();
					}

					ImGui::BeginDisabled(!current_player->block_join);

					if (ImGui::BeginCombo("Block Join Alert", block_join_reasons[current_player->block_join_reason]))
					{
						block_join_reason_t i = block_join_reason_t::UNK_0;
						for (const auto& reason_str : block_join_reasons)
						{
							if (reason_str != "")
							{
								const bool is_selected = current_player->block_join_reason == i;

								if (ImGui::Selectable(reason_str, is_selected))
								{
									current_player->block_join_reason = i;
								}

								if (is_selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							i++;
						}
						ImGui::EndCombo();
					}
					ImGui::EndDisabled();

					if (ImGui::BeginCombo("Chat Command Permissions",
					        COMMAND_ACCESS_LEVELS[g_player_service->get_selected()->command_access_level.value_or(
					            g.session.chat_command_default_access_level)]))
					{
						for (const auto& [type, name] : COMMAND_ACCESS_LEVELS)
						{
							if (ImGui::Selectable(name,
							        type == g_player_service->get_selected()->command_access_level.value_or(g.session.chat_command_default_access_level)))
							{
								g.session.chat_command_default_access_level = type;
								g_player_database_service->get_or_create_player(g_player_service->get_selected())->command_access_level = type;
								g_player_database_service->save();
							}
							if (type == g_player_service->get_selected()->command_access_level.value_or(g.session.chat_command_default_access_level))
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					ImGui::TreePop();
				}
			}
		}

		if (cped != nullptr)
		{
			if (ImGui::TreeNode("Ped Info"))
			{
				auto pos = cped->m_navigation->get_position();
				ImGui::Text("Health: %f / %f", cped->m_health, cped->m_maxhealth);
				ImGui::SameLine();
				ImGui::Text("Armor: %f", cped->m_armor);
				ImGui::Text("Pos X: %f, Y: %f, Z: %f", pos->x, pos->y, pos->z);

				ImGui::Text("Distance: %f", math::distance_between_vectors(misc::fvector3_to_Vector3(*g_local_player->get_position()), misc::fvector3_to_Vector3(*cped->get_position())));
				ImGui::TreePop();
			}

			if (player_info != nullptr && cped->m_weapon_manager != nullptr)
			{
				if (ImGui::TreeNode("Weapon Info"))
				{
					ImGui::Text("Weapon Name: %s",
					    g_gta_data_service->weapon_by_hash(cped->m_weapon_manager->m_selected_weapon_hash).m_name.c_str());
					ImGui::Text("Weapon Display Name: %s",
					    g_gta_data_service->weapon_by_hash(cped->m_weapon_manager->m_selected_weapon_hash).m_display_name.c_str());
					if (cped->m_weapon_manager->m_weapon_info != nullptr)
					{
						ImGui::Text("Weapon Damage: %f", cped->m_weapon_manager->m_weapon_info->m_damage);
						ImGui::Text("Weapon Damage Mult: %f", cped->m_weapon_manager->m_weapon_info->m_network_player_damage_modifier);
					}
					else if (cped->m_weapon_manager->m_vehicle_weapon_info != nullptr)
					{
						ImGui::Text("Weapon Damage: %f", cped->m_weapon_manager->m_vehicle_weapon_info->m_damage);
						ImGui::Text("Weapon Damage Mult: %f", cped->m_weapon_manager->m_vehicle_weapon_info->m_network_player_damage_modifier);
					}
					ImGui::Text("Mele Damage Mult: %f", player_info->m_melee_weapon_damage_mult);
					ImGui::TreePop();
				}
			}

			if (cped->m_vehicle != nullptr)
			{
				if (ImGui::TreeNode("Vehicle Info"))
				{
					const auto vehicle = cped->m_vehicle;
					ImGui::Text("Health: %f / %f", vehicle->m_health, vehicle->m_maxhealth);
					ImGui::Text("Engine Health: %f", vehicle->m_engine_health);
					ImGui::Text("Body Health: %f", vehicle->m_body_health);
					ImGui::Text("Petrol Tank Health: %f", vehicle->m_petrol_tank_health);
					ImGui::SameLine();
					ImGui::Text("Deform God: %s", vehicle->m_deform_god ? "Yes" : "No");
					ImGui::Text("Num Passengers %i", vehicle->m_num_of_passengers);
					ImGui::Text("Boost: %f", vehicle->m_boost);
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::TreeNode("Global Stats"))
		{
			auto id = g_player_service->get_selected()->id();

			if (id != -1)
			{
				auto& stats     = scr_globals::gpbd_fm_1.as<GPBD_FM*>()->Entries[id].PlayerStats;
				auto& boss_goon = scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[id].BossGoon;

				if (boss_goon.Language >= 0 && boss_goon.Language < 13)
					ImGui::Text("Language: %s", languages[boss_goon.Language].name);

				ImGui::Text("CEO Name: %s", boss_goon.GangName);
				ImGui::Text("MC Name: %s", boss_goon.ClubhouseName);
				ImGui::Text("Money In Wallet: %d", stats.WalletBalance);
				ImGui::Text("Money In Bank: %d", stats.Money - stats.WalletBalance);
				ImGui::Text("Total Money: %d", stats.Money);
				ImGui::Text("Rank: %d (RP %d)", stats.Rank, stats.RP);
				ImGui::Text("K/D Ratio: %f", stats.KdRatio);
				ImGui::Text("Kills On Players: %d", stats.KillsOnPlayers);
				ImGui::Text("Deaths By Players: %d", stats.DeathsByPlayers);
				ImGui::Text("Prostitutes Frequented: %d", stats.ProstitutesFrequented);
				ImGui::Text("Lap Dances Bought: %d", stats.LapDancesBought);
				ImGui::Text("Missions Created: %d", stats.MissionsCreated);
				ImGui::Text("Meltdown Complete: %s",
				    scr_globals::gpbd_fm_1.as<GPBD_FM*>()->Entries[id].MeltdownComplete ? "Yes" : "No"); // curious to see if anyone has actually played singleplayer
				ImGui::Text("Allows Spectating: %s", stats.CanSpectate ? "Yes" : "No");
			}

			ImGui::TreePop();
		}
	}
}
