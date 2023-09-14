/**
 * @file components.hpp
 * @brief YimMenu wrapers for ImGui functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "backend/command.hpp"
#include "backend/float_command.hpp"
#include "backend/int_command.hpp"
#include "backend/looped_command.hpp"
#include "backend/player_command.hpp"
#include "fiber_pool.hpp"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace big
{
	struct navigation_struct;
	enum class tabs;

	class components
	{
		static void custom_text(const std::string_view, ImFont*);

	public:
		static bool nav_button(const std::string_view);
		static bool menu_item(const std::string_view);
		static void icon(const std::string_view);
		static void help_marker(const std::string_view);
		static void small_text(const std::string_view);
		static void sub_title(const std::string_view);
		static void title(const std::string_view);
		static void markdown(const std::string_view);
		static void menu_item(const std::string_view, std::function<void()>);

		static bool input_text_with_hint(const std::string_view label, const std::string_view hint, char* buf, size_t buf_size, ImGuiInputTextFlags flag = ImGuiInputTextFlags_None, std::function<void()> cb = nullptr);
		static bool input_text_with_hint(const std::string_view label, const std::string_view hint, std::string& buf, ImGuiInputTextFlags flag = ImGuiInputTextFlags_None, std::function<void()> cb = nullptr);
		static void input_text(const std::string_view label, char* buf, size_t buf_size, ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_None, std::function<void()> cb = nullptr);

		static bool selectable(const std::string_view, bool);
		static bool selectable(const std::string_view, bool, ImGuiSelectableFlags);
		static void selectable(const std::string_view, bool, std::function<void()>);
		static void selectable(const std::string_view, bool, ImGuiSelectableFlags, std::function<void()>);

		static bool script_patch_checkbox(const std::string_view text, bool* option, const std::string_view tooltip = "");

		static void options_modal(std::string element_name, std::function<void()> render_elements, bool sameline = true, std::string custom_button_name = "Options");

		template<template_str cmd_str, ImVec2 size = ImVec2(0, 0), ImVec4 color = ImVec4(0.172f, 0.380f, 0.909f, 1.f)> // TODO: Use GUI Color.
		
		static void command_button(const std::vector<uint64_t> args = {}, std::optional<const std::string_view> label_override = std::nullopt)
		{
			static command* command = command::get(rage::consteval_joaat(cmd_str.value));
			if (command == nullptr)
				return ImGui::Text("INVALID COMMAND");

			if (ImGui::Button(label_override.value_or(command->get_label()).data()))
			{
				command_arguments _args(args);
				command->call(_args);
			}

			ImGui::SameLine();
			help_marker(command->get_description());
		}

		template<template_str cmd_str, ImVec2 size = ImVec2(0, 0), ImVec4 color = ImVec4(0.172f, 0.380f, 0.909f, 1.f)> // TODO: Use GUI Color.
		static void player_command_button(player_ptr player = g_player_service->get_selected(), const std::vector<uint64_t> args = {}, std::optional<const std::string_view> label_override = std::nullopt)
		{
			static player_command* command = dynamic_cast<player_command*>(command::get(rage::consteval_joaat(cmd_str.value)));
			if (command == nullptr)
				return ImGui::Text("INVALID COMMAND");

			if (ImGui::Button(label_override.value_or(command->get_label()).data()))
				command->call(player, args);

			ImGui::SameLine();
			help_marker(command->get_description());
		}

		template<template_str cmd_str>
		static bool command_checkbox(std::optional<const std::string_view> label_override = std::nullopt)
		{
			static bool_command* command = dynamic_cast<bool_command*>(command::get(rage::consteval_joaat(cmd_str.value)));
			if (command == nullptr)
			{
				ImGui::Text("INVALID COMMAND");
				return false;
			}

			bool updated;
			if (updated = ImGui::Checkbox(label_override.value_or(command->get_label()).data(), &command->is_enabled()))
				command->refresh();

			ImGui::SameLine();
			help_marker(command->get_description());

			return updated;
		}

		template<template_str cmd_str>
		static void command_int_slider(std::optional<const std::string_view> label_override = std::nullopt)
		{
			static int_command* command = (int_command*)command::get(rage::consteval_joaat(cmd_str.value));
			if (command == nullptr)
				return ImGui::Text("INVALID COMMAND");

			ImGui::SliderInt(label_override.value_or(command->get_label()).data(),
			    &command->get_value(),
			    command->get_lower_bound(),
			    command->get_upper_bound());

			help_marker(command->get_description());
		}

		template<template_str cmd_str>
		static void command_float_slider(std::optional<const std::string_view> label_override = std::nullopt)
		{
			static float_command* command = (float_command*)command::get(rage::consteval_joaat(cmd_str.value));
			if (command == nullptr)
				return ImGui::Text("INVALID COMMAND");

			ImGui::SliderFloat(label_override.value_or(command->get_label()).data(),
			    &command->get_value(),
			    command->get_lower_bound(),
			    command->get_upper_bound());

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(command->get_description().c_str());
		}

		template<ImVec2 size = ImVec2(0, 0), ImVec4 color = ImVec4(0.172f, 0.380f, 0.909f, 1.f)> // TODO: Use GUI Color.
		static bool button(const std::string_view text)
		{
			bool status = false;

			if constexpr (color.x != 0.172f || color.y != 0.380f || color.z != 0.909f || color.w != 1.0f)
				ImGui::PushStyleColor(ImGuiCol_Button, color);

			status = ImGui::Button(text.data(), size);

			if constexpr (color.x != 0.172f || color.y != 0.380f || color.z != 0.909f || color.w != 1.0f)
				ImGui::PopStyleColor(1);
			return status;
		}

		template<ImVec2 size = ImVec2(0, 0), ImVec4 color = ImVec4(0.172f, 0.380f, 0.909f, 1.f)> // TODO: Use GUI Color.
		static void button(const std::string_view text, std::function<void()> cb)
		{
			if (button<size, color>(text))
			{
				g_fiber_pool->queue_job(cb);
			}
		}

		template<ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f)>
		static void overlay_indicator(const std::string_view text, bool value)
		{
			ImGui::Text(std::format("{}: ", text).data());
			ImGui::SameLine(180);
			ImGui::TextColored(value ? green : red, value ? "Enabled" : "Disabled");
		}

		template<typename PredicateFn, typename ComponentsFn>
		static void disable_unless(PredicateFn predicate_fn, ComponentsFn components_fn)
		{
			auto const result = predicate_fn();
			if (!result)
				ImGui::BeginDisabled(true);
			components_fn();
			if (!result)
				ImGui::EndDisabled();
		}
	};
}
