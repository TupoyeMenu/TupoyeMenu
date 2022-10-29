#include "gui/components/components.hpp"
#include "natives.hpp"
#include "util/system.hpp"
#include "view_debug.hpp"

namespace big
{
	void debug::misc()
	{
		if (ImGui::BeginTabItem("Misc"))
		{
			if (components::button("Dump entrypoints"))
			{
				system::dump_entry_points();
			}

			components::button("Network Bail", []
			{
				NETWORK::NETWORK_BAIL(16, 0, 0);
			});

			components::button("Network Session Host", []
			{
				NETWORK::NETWORK_SESSION_HOST(1, 32, TRUE);
			});

			ImGui::EndTabItem();
		}
	}
}