#include "core/data/block_join_reasons.hpp"
#include "core/data/command_access_levels.hpp"
#include "core/data/infractions.hpp"
#include "fiber_pool.hpp"
#include "pointers.hpp"
#include "services/player_database/player_database_service.hpp"
#include "services/players/player_service.hpp"
#include "util/session.hpp"
#include "views/view.hpp"

namespace big
{
	persistent_player current_player;

	void view::player_database()
	{
		static char name_buf[32];
		static char search[64];

		ImGui::SetNextItemWidth(300.f);
		components::input_text_with_hint("Player", "Search", search, sizeof(search), ImGuiInputTextFlags_None);

		if (ImGui::ListBoxHeader("###players", {180, static_cast<float>(*g_pointers->m_resolution_y - 400 - 38 * 4)}))
		{
			auto& item_arr = g_player_database_service->get_players();
			if (item_arr.size() > 0)
			{
				std::string lower_search = search;
				std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), tolower);

				for (auto& item : item_arr)
				{
					auto& player = item.second;

					std::string name = player.name;
					std::transform(name.begin(), name.end(), name.begin(), ::tolower);

					if (lower_search.empty() || name.find(lower_search) != std::string::npos)
					{
						ImGui::PushID(item.first);

						float circle_size = 7.5f;
						auto cursor_pos   = ImGui::GetCursorScreenPos();
						auto plyr_state   = player.online_state;

						//render status circle
						ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(cursor_pos.x + 4.f + circle_size, cursor_pos.y + 4.f + circle_size),
						    circle_size,
						    ImColor(plyr_state == PlayerOnlineStatus::ONLINE  ? ImVec4(0.f, 1.f, 0.f, 1.f) :
						            plyr_state == PlayerOnlineStatus::OFFLINE ? ImVec4(1.f, 0.f, 0.f, 1.f) :
						            plyr_state == PlayerOnlineStatus::UNKNOWN ? ImVec4(.5f, .5f, .5f, 1.0f) :
						                                                        ImVec4(.5f, .5f, .5f, 1.0f)));

						//we need some padding
						ImVec2 cursor = ImGui::GetCursorPos();
						ImGui::SetCursorPos(ImVec2(cursor.x + 25.f, cursor.y));

						if (components::selectable(player.name, &player == g_player_database_service->get_selected()))
						{
							g_player_database_service->set_selected(&player);
							current_player = player;
							strncpy(name_buf, current_player.name.data(), sizeof(name_buf));
						}

						ImGui::PopID();
					}
				}
			}
			else
			{
				ImGui::Text("No stored players");
			}

			ImGui::ListBoxFooter();
		}

		if (auto selected = g_player_database_service->get_selected())
		{
			ImGui::SameLine();
			if (ImGui::BeginChild("###selected_player", {500, static_cast<float>(*g_pointers->m_resolution_y - 388 - 38 * 4)}, false, ImGuiWindowFlags_NoBackground))
			{
				if (ImGui::InputText("Name", name_buf, sizeof(name_buf)))
				{
					current_player.name = name_buf;
				}

				ImGui::InputScalar("Rockstar ID", ImGuiDataType_S64, &current_player.rockstar_id);
				ImGui::Checkbox("Is Modder", &current_player.is_modder);
				ImGui::Checkbox("Force Allow Join", &current_player.force_allow_join);
				ImGui::Checkbox("Block Join", &current_player.block_join);

				if (ImGui::BeginCombo("Block Join Alert", block_join_reasons[current_player.block_join_reason]))
				{
					for (const auto& reason : block_join_reasons)
					{
						if (ImGui::Selectable(reason.second, reason.first == current_player.block_join_reason))
						{
							current_player.block_join_reason = reason.first;
						}

						if (reason.first == current_player.block_join_reason)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::SameLine();
				components::help_marker("Only works as host");

				if (ImGui::BeginCombo("Chat Command Permissions",
				        COMMAND_ACCESS_LEVELS[current_player.command_access_level.value_or(g.session.chat_command_default_access_level)]))
				{
					for (const auto& [type, name] : COMMAND_ACCESS_LEVELS)
					{
						if (ImGui::Selectable(name, type == current_player.command_access_level.value_or(g.session.chat_command_default_access_level)))
						{
							current_player.command_access_level = type;
						}

						if (type == current_player.command_access_level.value_or(g.session.chat_command_default_access_level))
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (!current_player.infractions.empty())
				{
					ImGui::Text("Infractions:");

					for (auto& infraction : current_player.infractions)
					{
						ImGui::BulletText(infraction_desc[(Infraction)infraction]);
					}
				}

				components::button("Join Session", [] {
					session::join_by_rockstar_id(current_player.rockstar_id);
				});

				if (ImGui::Button("Save"))
				{
					if (current_player.rockstar_id != selected->rockstar_id)
						g_player_database_service->update_rockstar_id(selected->rockstar_id, current_player.rockstar_id);

					*selected = current_player;
					g_player_database_service->save();
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove"))
				{
					g_player_database_service->remove_rockstar_id(selected->rockstar_id);
				}
			}
			ImGui::EndChild();
		}

		if (ImGui::Button("Remove All"))
		{
			g_player_database_service->set_selected(nullptr);
			g_player_database_service->get_players().clear();
			g_player_database_service->save();
		}

		ImGui::SameLine();

		components::button("Reload Player Online States", [] {
			g_player_database_service->update_player_states();
		});

		ImGui::SeparatorText("New Entry");

		static char new_name[64];
		static int64_t new_rockstar_id;

		components::input_text("Name", new_name, sizeof(new_name));
		ImGui::InputScalar("Rockstar ID", ImGuiDataType_S64, &new_rockstar_id);

		if (ImGui::Button("Add"))
		{
			g_player_database_service->get_players()[new_rockstar_id] = persistent_player(new_name, new_rockstar_id);
			g_player_database_service->save();
		}
	}
}
