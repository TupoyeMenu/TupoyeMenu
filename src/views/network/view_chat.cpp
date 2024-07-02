#include "core/scr_globals.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "views/view.hpp"
#include "util/chat.hpp"
#include "core/data/command_access_levels.hpp"

namespace big
{
	void view::chat()
	{
		static char msg[256];
		ImGui::Checkbox("Use Spam Timer", &g.session.use_spam_timer);
		if (g.session.use_spam_timer)
		{
			ImGui::SliderFloat("Spam Timer", &g.session.spam_timer, 0.5f, 5.0f);
			ImGui::SliderInt("Spam Text Length", &g.session.spam_length, 1, 256);
		}
		ImGui::Checkbox("Log Chat Messages", &g.session.log_chat_messages);
		components::input_text_with_hint("##message", "Chat message", msg, sizeof(msg));

		ImGui::Checkbox("Is Team", &g.session.is_team);
		ImGui::SameLine();
		components::button("Send", [] {
			if (const auto net_game_player = gta_util::get_network_player_mgr()->m_local_net_player; net_game_player)
			{
				chat::send_message(msg, nullptr, true, g.session.is_team);
			}
		});

		ImGui::Separator();

		ImGui::Checkbox("Chat Commands", &g.session.chat_commands);
		if (g.session.chat_commands)
		{
			components::small_text("Default Command Permissions");
			if (ImGui::BeginCombo("##defualtchatcommands", COMMAND_ACCESS_LEVELS[g.session.chat_command_default_access_level]))
			{
				for (const auto& [type, name] : COMMAND_ACCESS_LEVELS)
				{
					if (ImGui::Selectable(name, type == g.session.chat_command_default_access_level))
					{
						g.session.chat_command_default_access_level = type;
					}

					if (type == g.session.chat_command_default_access_level)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}
		}
	}
}
