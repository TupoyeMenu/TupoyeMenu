#include "core/data/language_codes.hpp"
#include "pointers.hpp"
#include "thread_pool.hpp"
#include "views/view.hpp"
#include "script_mgr.hpp"

namespace big
{
	void scripts_popupmodal()
	{
		ImGui::BeginGroup();
		components::sub_title("Scripts");
		ImGui::SameLine(ImGui::GetWindowWidth() - 100);
		if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
		ImGui::Spacing();
		components::sub_title("These scripts are responsible for all looped features.\nOnly disable if you know what you are doing.");

		g_script_mgr.for_each_script([](const auto& script) {
			if (script->is_toggleable())
				if (ImGui::Checkbox(script->name(), script->toggle_ptr()))
					g_notification_service->push(std::string(script->name()).append(" script"), script->is_enabled() ? "Resumed" : "Halted");
		});

		ImGui::EndGroup();
	}

	void view::settings()
	{
		const auto& language_entries = g_translation_service.available_translations();
		const auto& current_pack     = g_translation_service.current_language_pack();

		ImGui::SeparatorText("SETTINGS_LANGUAGES"_T.data());

		if (ImGui::BeginCombo("Menu Language", language_entries.at(current_pack).name.c_str()))
		{
			for (auto& i : language_entries)
			{
				if (ImGui::Selectable(i.second.name.c_str(), i.first == current_pack))
					g_translation_service.select_language_pack(i.first);

				if (i.first == current_pack)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Game Language", languages[*g_pointers->m_gta.m_language].name))
		{
			for (auto& language : languages)
			{
				if (ImGui::Selectable(language.name, language.id == *g_pointers->m_gta.m_language))
				{
					*g_pointers->m_gta.m_language = language.id;

					g_fiber_pool->queue_job([] {
						g_pointers->m_gta.m_update_language(true);
					});
				}

				if (language.id == *g_pointers->m_gta.m_language)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if (components::button("Force Update Languages"))
		{
			g_thread_pool->push([] {
				g_translation_service.update_language_packs();

				g_notification_service->push_success("Translations", "Finished updating translations.");
			});
		}

		ImGui::SeparatorText("SETTINGS_MISC"_T.data());
		ImGui::Checkbox("SETTINGS_MISC_DEV_DLC"_T.data(), &g.settings.dev_dlc);
		ImGui::Checkbox("Enable Cable Cars", &g.tunables.cable_cars);

		ImGui::Separator();

		if (ImGui::Button("Manage scripts"))
			ImGui::OpenPopup("Scripts");
		
		ImGui::SetNextWindowPos({ 780,228 }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal("Scripts", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			scripts_popupmodal();

			ImGui::EndPopup();
		}

		if (ImGui::Button("Reset Settings"))
		{
			g.write_default_config();
			g.load();
		}
	}
}