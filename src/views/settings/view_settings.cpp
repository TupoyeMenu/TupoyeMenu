
#include "core/data/language_codes.hpp"
#include "pointers.hpp"
#include "renderer/alphabet_types.hpp"
#include "renderer/renderer.hpp"
#include "script_mgr.hpp"
#include "thread_pool.hpp"
#include "core/data/alphabet_types.hpp"
#include "views/view.hpp"

namespace big
{
	void scripts_popupmodal()
	{
		ImGui::BeginGroup();
		components::sub_title("Scripts");
		ImGui::SameLine(ImGui::GetWindowWidth() - 100);
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::Spacing();
		components::sub_title("These scripts are responsible for all looped features.\nOnly disable if you know what you are doing.");

		g_script_mgr.for_each_script([](const auto& script) {
			if (script->is_toggleable())
				if (ImGui::Checkbox(script->name(), script->toggle_ptr()))
					g_notification_service.push(std::string(script->name()).append(" script"), script->is_enabled() ? "Resumed" : "Halted");
		});

		ImGui::EndGroup();
	}

	void view::settings()
	{
		ImGui::TextUnformatted("Misc");
		ImGui::Checkbox("Enable Dev DLC", &g.settings.dev_dlc);
		ImGui::Checkbox("Enable Cable Cars", &g.tunables.cable_cars);

		ImGui::Separator();

		if (ImGui::Button("Manage scripts"))
			ImGui::OpenPopup("Scripts");

		ImGui::SetNextWindowPos({780, 228}, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal("Scripts", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			scripts_popupmodal();

			ImGui::EndPopup();
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

		if (ImGui::BeginCombo("Menu Alphabet", alphabet_types[(int)g.window.alphabet]))
		{
			for (int i = 1; i < alphabet_types.size(); i++)
			{
				if (ImGui::Selectable(alphabet_types[i], i == (int)g.window.alphabet))
				{
					g.window.alphabet = (eAlphabetType)i;
					g_renderer.get_font_mgr().update_required_alphabet_type(g.window.alphabet);
				}
				if (i == (int)g.window.alphabet)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Reset Settings"))
		{
			g.write_default_config();
			g.load();
		}
	}
}
