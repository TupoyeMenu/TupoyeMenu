/**
 * @file view_players.cpp
 * @brief Player list.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#define IMGUI_DEFINE_PLACEMENT_NEW
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "fiber_pool.hpp"
#include "fonts/fonts.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "services/player_database/player_database_service.hpp"
#include "services/players/player_service.hpp"
#include "views/view.hpp"

#include <imgui_internal.h>

namespace big
{
	bool has_scrollbar = false;
	static void player_button(const player_ptr& plyr)
	{
		if (plyr == nullptr)
			return;

		bool selected_player = plyr == g_player_service->get_selected();

		// generate icons string
		std::string player_icons;
		if (plyr->is_host())
			player_icons += FONT_ICON_HOST;
		if (plyr->is_friend())
			player_icons += FONT_ICON_FRIEND;
		if (const auto ped = plyr->get_ped(); ped != nullptr)
		{
			if (ped->m_ped_task_flag & (uint8_t)ePedTask::TASK_DRIVING)
				player_icons += FONT_ICON_VEHICLE;
			if (ped->m_damage_bits & (uint32_t)eEntityProofs::GOD)
				player_icons += FONT_ICON_GODMODE;
		}
		if (plyr->is_modder)
			player_icons += FONT_ICON_MODDER;

		const auto player_iconsc    = player_icons.c_str();
		const auto player_icons_end = player_iconsc + player_icons.size();

		// calculate icons width
		const auto window = ImGui::GetCurrentWindow();
		ImGui::PushFont(g.window.font_icon);
		const auto icons_size = ImGui::CalcTextSize(player_iconsc, player_icons_end);
		const ImVec2 icons_pos(window->DC.CursorPos.x + (ImGui::GetWindowSize().x - ImGui::GetStyle().FramePadding.x * 2) - 32.0f - icons_size.x, window->DC.CursorPos.y + 2.0f);
		const ImRect icons_box(icons_pos, icons_pos + icons_size);
		ImGui::PopFont();

		if (plyr->is_admin)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.67f, 0.f, 1.f));
		else if (plyr->is_modder)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.1f, 0.1f, 1.f));
		else if (plyr->is_trusted)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.67f, 0.1f, 1.f));

		if (selected_player)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.29f, 0.45f, 0.69f, 1.f));

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0.0, 0.5});
		ImGui::PushID(plyr->id());
		if (ImGui::Button(plyr->get_name(), {ImGui::GetWindowSize().x - ImGui::GetStyle().FramePadding.x * 2, 0.f}))
		{
			g_player_service->set_selected(plyr);
			g.window.player = true;
		}
		if (ImGui::IsItemHovered()
		    && g_player_database_service->get_player_by_rockstar_id(plyr->get_net_data()->m_gamer_handle.m_rockstar_id) != nullptr)
		{
			auto sorted_player =
			    g_player_database_service->get_player_by_rockstar_id(plyr->get_net_data()->m_gamer_handle.m_rockstar_id);

			if (!sorted_player->infractions.empty())
			{
				ImGui::BeginTooltip();
				for (auto infraction : sorted_player->infractions)
					ImGui::BulletText("%s", sorted_player->get_infraction_description(infraction));
				ImGui::EndTooltip();
			}
		}

		ImGui::PopID();
		ImGui::PopStyleVar();

		if (selected_player)
			ImGui::PopStyleColor();

		if (plyr->is_admin || plyr->is_modder || plyr->is_trusted)
			ImGui::PopStyleColor();

		// render icons on top of the player button
		ImGui::PushFont(g.window.font_icon);
		ImGui::RenderTextWrapped(icons_box.Min, player_iconsc, player_icons_end, icons_size.x);
		ImGui::PopFont();
	}

	void view::players()
	{
		// player count does not include ourself that's why +1
		const auto player_count = g_player_service->players().size() + 1;

		if (!*g_pointers->m_gta.m_is_session_started && player_count < 2)
			return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {2.0f, 2.0f});

		if (ImGui::Begin(std::format("Playerlist {}###Playerlist", player_count).c_str(), &g.window.users))
		{
			float window_height = (ImGui::CalcTextSize("A").y + ImGui::GetStyle().ItemInnerSpacing.y * 2 + 6.0f) * player_count;

			ImGui::PushStyleColor(ImGuiCol_FrameBg, {0.f, 0.f, 0.f, 0.f});
			ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, {0.f, 0.f, 0.f, 0.f});

			if (ImGui::BeginListBox("##players", {ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, window_height}))
			{
				ImGui::SetScrollX(0);
				player_button(g_player_service->get_self());

				if (player_count > 1)
					ImGui::Separator();

				for (const auto& [_, player] : g_player_service->players())
					player_button(player);

				ImGui::EndListBox();
			}
			ImGui::PopStyleColor(2);
		}

		ImGui::PopStyleVar();
		ImGui::End();
	}
}
