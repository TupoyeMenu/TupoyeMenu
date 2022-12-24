#include "views/view.hpp"
#include "fiber_pool.hpp"
#include "logger.hpp"

namespace big
{
	static ImVec4 get_color(const LEVELS level)
	{
		switch (level.value)
		{
		case g3::kDebugValue:
			return ImVec4(0.0f, 0.44f, 0.85f, 1.0f);
		case g3::kInfoValue:
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		case g3::kWarningValue:
			return ImVec4(1.0f, 0.7f, 0.0f, 1.0f);
		}
		return g3::internal::wasFatal(level) ? ImVec4(1.0f, 0.4f, 0.4f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	static size_t iLastLogCount = 0;
	void view::log()
	{
		if (ImGui::Begin("Log", &g.window.log))
		{
			auto msgs = g_log->get_messages();

			ImGuiListClipper clipper;
			clipper.Begin((int)msgs.size());
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild("##scrolling_region_log", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, get_color(msgs[i].level));
					ImGui::TextUnformatted(msgs[i].m_message.c_str());
					ImGui::PopStyleColor();
				}
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::PopStyleVar();
			ImGui::EndChild();

			ImGui::Separator();

			ImGui::InputTextMultiline("###console_command_input", &g.chat.message, ImVec2(-80, ImGui::GetTextLineHeightWithSpacing()));
			ImGui::SameLine();
			ImGui::Button("Submit");

			ImGui::End();
		}
	}
}
