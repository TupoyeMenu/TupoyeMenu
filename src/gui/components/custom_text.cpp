#include "gui/components/components.hpp"

namespace big
{
	void components::custom_text(const std::string_view text, ImFont* font)
	{
		ImGui::PushFont(font);
		ImGui::TextWrapped("%s", text.data());
		ImGui::PopFont();
	}
}