#include "main_tabs.hpp"

namespace big
{
	void tab_main::tab_tunables()
	{
		if (ImGui::BeginTabItem("Tunables"))
		{
			ImGui::Checkbox("Disable Phone", &g.tunables.disable_phone);
			ImGui::Checkbox("No Idle Kick", &g.tunables.no_idle_kick);
			ImGui::Checkbox("No Spam Help", &g.tunables.no_spam_help);
			ImGui::Checkbox("Block Join Requests", &g.tunables.BLOCK_JOIN_REQUESTS);

			ImGui::EndTabItem();
		}
	}
}