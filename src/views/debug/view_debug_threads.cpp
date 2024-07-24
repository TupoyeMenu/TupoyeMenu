/**
 * @file view_debug_threads.cpp
 * @brief In game .ysc script thread monitor.
 */

#include "core/data/all_script_names.hpp"
#include "core/data/stack_sizes.hpp"
#include "fiber_pool.hpp"
#include "gta/script_handler.hpp"
#include "gui/components/components.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "util/scripts.hpp"
#include "views/view.hpp"

#include <script/GtaThread.hpp>

static GtaThread* selected_thread;

static int selected_stack_size             = 128;
static int free_stacks                     = -1;
static const char* selected_stack_size_str = "MULTIPLAYER_MISSION";
static const char* selected_script         = "";

static std::chrono::high_resolution_clock::time_point last_stack_update_time{};

namespace
{
	static void update_free_stacks_count()
	{
		free_stacks = MISC::GET_NUMBER_OF_FREE_STACKS_OF_THIS_SIZE(selected_stack_size);
	}
}

namespace big
{
	void view::debug_threads()
	{
		if (!g_pointers->m_gta.m_script_threads)
		{
			selected_thread = nullptr;
			return;
		}

		if (ImGui::BeginListBox("##threads", ImVec2(250, -ImGui::GetFrameHeight())))
		{
			for (auto script : *g_pointers->m_gta.m_script_threads)
			{
				if (script)
				{
					if (script->m_context.m_state != rage::eThreadState::killed && script->m_context.m_stack_size == 0)
						continue;

					ImGui::PushID(script->m_context.m_thread_id);

					if (script->m_context.m_state == rage::eThreadState::killed)
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.1f, 0.1f, 1.f));

					if (script->m_context.m_state >= rage::eThreadState::unk_3)
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.8f, 0.1f, 1.f));

					if (ImGui::Selectable(script->m_name, selected_thread == script))
					{
						selected_thread = script;
					}

					if (selected_thread == script)
						ImGui::SetItemDefaultFocus();

					if (script->m_context.m_state == rage::eThreadState::killed || script->m_context.m_state >= rage::eThreadState::unk_3)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
			}
			ImGui::EndListBox();
		}

		ImGui::SameLine();
		ImGui::BeginGroup();

		if (selected_thread)
		{
			auto net_handler = reinterpret_cast<CGameScriptHandlerNetComponent*>(selected_thread->m_net_component);

			if (net_handler)
			{
				auto host = net_handler->get_host();
				if (host)
				{
					ImGui::Text("Script Host: %s", host->get_name());

					if (!net_handler->is_local_player_host())
					{
						if (ImGui::SmallButton("Take Control"))
						{
							g_fiber_pool->queue_job([net_handler] {
								net_handler->send_host_migration_event(g_player_service->get_self()->get_net_game_player());
							});
						}
					}
				}
			}
			ImGui::Combo("State", (int*)&selected_thread->m_context.m_state, "RUNNING\0WAITING\0KILLED\0PAUSED\0STATE_4\0");

			ImGui::TextUnformatted("Script Pointer: ");
			ImGui::SameLine();
			if (ImGui::SmallButton(std::format("0x{:X}", (DWORD64)selected_thread).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", (DWORD64)selected_thread).c_str());

			ImGui::Text("m_safe_for_network_game: %s", selected_thread->m_safe_for_network_game ? "Yes" : "No");
			ImGui::Text("m_can_be_paused: %s", selected_thread->m_can_be_paused ? "Yes" : "No");
			ImGui::TextUnformatted("Stack Pointer: ");
			ImGui::SameLine();
			if (ImGui::SmallButton(std::format("0x{:X}", (DWORD64)selected_thread->m_stack).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", (DWORD64)selected_thread->m_stack).c_str());
			ImGui::Text("Internal Stack Pointer: %d Stack Size: %d",
			    selected_thread->m_context.m_stack_pointer,
			    selected_thread->m_context.m_stack_size);
			ImGui::Text("Instruction Pointer: %X", selected_thread->m_context.m_instruction_pointer);

			if (selected_thread->m_context.m_state == rage::eThreadState::killed)
				ImGui::Text("Exit Reason: %s", selected_thread->m_exit_message);

			if (ImGui::Button("Kill"))
			{
				if (selected_thread->m_context.m_stack_size != 0)
					selected_thread->kill();

				selected_thread->m_context.m_state = rage::eThreadState::killed;
			}
		}

		ImGui::SeparatorText("Start New Script");

		static std::string search_script = "";
		components::input_text_with_hint("Script", "Search", search_script, ImGuiInputTextFlags_None);

		if (ImGui::BeginListBox("##scripts"))
		{
			std::string lower_search = search_script;
			std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), ::tolower);

			for (auto& script : all_script_names)
			{
				std::string lower_script = script;
				std::transform(lower_script.begin(), lower_script.end(), lower_script.begin(), ::tolower);

				if (lower_script.find(lower_search) != std::string::npos)
				{
					if (ImGui::Selectable(script, selected_script == script))
					{
						selected_script = script;
						search_script   = script;
					}
				}
			}
			ImGui::EndListBox();
		}
		if (ImGui::BeginCombo("Stack Size", selected_stack_size_str))
		{
			for (auto& p : stack_sizes)
			{
				if (ImGui::Selectable(std::format("{} ({})", p.first, p.second).data(), selected_stack_size == p.second))
				{
					selected_stack_size_str = std::format("{} ({})", p.first, p.second).data();
					selected_stack_size     = p.second;

					g_fiber_pool->queue_job([] {
						update_free_stacks_count();
					});
				}

				if (p.second == selected_stack_size)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Free Stacks: %d", free_stacks);

		components::button("Start", [] {
			auto hash = rage::joaat(selected_script);

			if (!SCRIPT::DOES_SCRIPT_WITH_NAME_HASH_EXIST(hash))
			{
				return;
			}

			if (MISC::GET_NUMBER_OF_FREE_STACKS_OF_THIS_SIZE(selected_stack_size) == 0)
			{
				g_notification_service.push_warning("Script Launcher", "No free stacks for this stack size");
			}

			while (!SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(hash))
			{
				SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH(hash);
				script::get_current()->yield();
			}

			SYSTEM::START_NEW_SCRIPT_WITH_NAME_HASH(hash, selected_stack_size);

			SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(hash);

			update_free_stacks_count();
		});

		ImGui::SameLine();

		components::button("Start With Launcher", [] {
			auto hash = rage::joaat(selected_script);
			auto idx  = scripts::launcher_index_from_hash(hash);

			if (idx == -1)
			{
				g_notification_service.push_warning("Script Launcher", "This script cannot be started using am_launcher");
				return;
			}

			scripts::start_launcher_script(hash);
		});

		ImGui::EndGroup();

		if (*g_pointers->m_gta.m_game_state != eGameState::Invalid && std::chrono::high_resolution_clock::now() - last_stack_update_time > 100ms)
		{
			last_stack_update_time = std::chrono::high_resolution_clock::now();
			g_fiber_pool->queue_job([] {
				update_free_stacks_count();
			});

			if (*g_pointers->m_gta.m_game_state != eGameState::Invalid && std::chrono::high_resolution_clock::now() - last_stack_update_time > 100ms)
			{
				last_stack_update_time = std::chrono::high_resolution_clock::now();
				g_fiber_pool->queue_job([] {
					update_free_stacks_count();
				});
			}
		}
	}
}