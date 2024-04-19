#include "gui.hpp"
#include "pointers.hpp"
#include "views/view.hpp"
#include "widgets/imgui_hotkey.hpp"

namespace big
{
	void select_menu_open_key()
	{
		ImGui::SeparatorText("Select a key to open TupoyeMenu:");
		if (ImGui::Hotkey("Menu Toggle", &g.settings.hotkeys.menu_toggle))
		{
			g_notification_service.push("Onboarding", "Successfully change the menu open key! Try to see if it works.");
		}
	}

	void developer_console()
	{
		ImGui::SeparatorText("Do you want to hide the developer console?");
		components::command_checkbox<"external_console">();
	}

	void view::onboarding()
	{
		static bool onboarding_open = false;
		if (g.settings.onboarding_complete)
		{
			return;
		}

		if (!onboarding_open)
		{
			g_gui->toggle(true);
			ImGui::OpenPopup("Welcome to TupoyeMenu!");
			onboarding_open = true;
		}

		const auto window_size     = ImVec2{600, 400};
		const auto window_position = ImVec2{(*g_pointers->m_gta.m_resolution_x - window_size.x) / 2,
		    (*g_pointers->m_gta.m_resolution_y - window_size.y) / 2};

		ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
		ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);

		if (ImGui::BeginPopupModal("Welcome to TupoyeMenu!"))
		{
			select_menu_open_key();
			developer_console();

			if (ImGui::Button("Close"))
			{
				g.settings.onboarding_complete = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
