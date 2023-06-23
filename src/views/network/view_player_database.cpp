/**
 * @file view_player_database.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

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
	char name_buf[32];
	char search[64];
	char note_buffer[1024];
	bool notes_dirty = false;
	std::shared_ptr<persistent_player> current_player;

	void draw_player_db_entry(std::shared_ptr<persistent_player> player, const std::string& lower_search)
	{
		std::string name = player->name;
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (lower_search.empty() || name.find(lower_search) != std::string::npos)
		{
			ImGui::PushID(player->rockstar_id);

			float circle_size = 7.5f;
			auto cursor_pos   = ImGui::GetCursorScreenPos();
			auto plyr_state   = player->online_state;

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

			if (components::selectable(player->name, player == g_player_database_service->get_selected()))
			{
				if (notes_dirty)
				{
					// Ensure notes are saved
					g_player_database_service->save();
					notes_dirty = false;
				}

				g_player_database_service->set_selected(player);
				current_player = player;
				strncpy(name_buf, current_player->name.data(), sizeof(name_buf));
				strncpy(note_buffer, current_player->notes.data(), sizeof(note_buffer));
			}

			ImGui::PopID();
		}
	}

	void view::player_database()
	{
		ImGui::SetNextItemWidth(300.f);
		components::input_text_with_hint("Player", "Search", search, sizeof(search), ImGuiInputTextFlags_None);

		if (ImGui::BeginListBox("###players", ImVec2(180, -ImGui::GetFrameHeight())))
		{
			auto& item_arr = g_player_database_service->get_sorted_players();
			if (item_arr.size() > 0)
			{
				std::string lower_search = search;
				std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), tolower);

				for (auto& player : item_arr | std::ranges::views::values)
				{
					if (player->online_state == PlayerOnlineStatus::ONLINE)
						draw_player_db_entry(player, lower_search);
				}

				for (auto& player : item_arr | std::ranges::views::values)
				{
					if (player->online_state != PlayerOnlineStatus::ONLINE)
						draw_player_db_entry(player, lower_search);
				}
			}
			else
			{
				ImGui::Text("No stored players");
			}

			ImGui::EndListBox();
		}

		if (auto selected = g_player_database_service->get_selected())
		{
			ImGui::SameLine();
			if (ImGui::BeginChild("###selected_player", ImVec2(500, -ImGui::GetFrameHeight()), false, ImGuiWindowFlags_NoBackground))
			{
				if (ImGui::InputText("Name", name_buf, sizeof(name_buf)))
				{
					current_player->name = name_buf;
				}

				if (ImGui::InputScalar("RID", ImGuiDataType_S64, &current_player->rockstar_id)
				    || ImGui::Checkbox("Is Modder", &current_player->is_modder)
				    || ImGui::Checkbox("Force Allow Join", &current_player->force_allow_join)
				    || ImGui::Checkbox("Block Join", &current_player->block_join)
					|| ImGui::Checkbox("Notify When Online", &current_player->notify_online))
				{
					if (current_player->rockstar_id != selected->rockstar_id)
						g_player_database_service->update_rockstar_id(selected->rockstar_id, current_player->rockstar_id);
					g_player_database_service->save();
				}

				if (ImGui::BeginCombo("Block Join Alert", block_join_reasons[current_player->block_join_reason]))
				{
					for (const auto& reason : block_join_reasons)
					{
						if (ImGui::Selectable(reason.second, reason.first == current_player->block_join_reason))
						{
							current_player->block_join_reason = reason.first;
							g_player_database_service->save();
						}

						if (reason.first == current_player->block_join_reason)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::SameLine();
				components::help_marker("Only works as host");

				if (ImGui::BeginCombo("Chat Command Permissions",
				        COMMAND_ACCESS_LEVELS[current_player->command_access_level.value_or(g.session.chat_command_default_access_level)]))
				{
					for (const auto& [type, name] : COMMAND_ACCESS_LEVELS)
					{
						if (ImGui::Selectable(name, type == current_player->command_access_level.value_or(g.session.chat_command_default_access_level)))
						{
							current_player->command_access_level = type;
							g_player_database_service->save();
						}

						if (type == current_player->command_access_level.value_or(g.session.chat_command_default_access_level))
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (!current_player->infractions.empty())
				{
					ImGui::Text("Infractions:");

					for (auto& infraction : current_player->infractions)
					{
						ImGui::BulletText(infraction_desc[(Infraction)infraction]);
					}
				}

				if (ImGui::InputTextMultiline("Notes", note_buffer, sizeof(note_buffer)))
				{
					current_player->notes = note_buffer;
					notes_dirty           = true;
				}

				components::button("Join Session", [] {
					session::join_by_rockstar_id(current_player->rockstar_id);
				});

				if (ImGui::Button("Save"))
				{
					if (current_player->rockstar_id != selected->rockstar_id)
						g_player_database_service->update_rockstar_id(selected->rockstar_id, current_player->rockstar_id);

					selected = current_player;
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
			g_player_database_service->get_sorted_players().clear();
			g_player_database_service->save();
		}

		ImGui::SameLine();

		components::button("Reload Player Online States", [] {
			g_player_database_service->update_player_states();
		});

		if (components::command_checkbox<"player_db_auto_update_states">())
			g_player_database_service->start_update_loop();

		ImGui::Separator();
		components::sub_title("New Entry");

		static char new_name[64];
		static int64_t new_rockstar_id;

		components::input_text("Name", new_name, sizeof(new_name));
		ImGui::InputScalar("Rockstar ID", ImGuiDataType_S64, &new_rockstar_id);

		if (ImGui::Button("Add"))
		{
			current_player = g_player_database_service->add_player(new_rockstar_id, new_name);
			g_player_database_service->save();
		}
	}
}
