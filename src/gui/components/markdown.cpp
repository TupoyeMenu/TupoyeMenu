#include "widgets/imgui_markdown.hpp"
#include "fonts/fonts.hpp"
#include "gui/components/components.hpp"

#include "shellapi.h"

namespace big
{
	void link_callback( ImGui::MarkdownLinkCallbackData data_ )
	{
		std::string url( data_.link, data_.linkLength );
		if( !data_.isImage )
		{
			ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
		}
	}

	void components::markdown(const std::string_view text)
	{
		ImGui::MarkdownConfig mdConfig{ link_callback, 0, 0, 0, { { g.window.font_title, true }, { g.window.font_sub_title, true }, { g.window.font_small, true } }, NULL };
		ImGui::Markdown( text.data(), text.length(), mdConfig );
	}
}