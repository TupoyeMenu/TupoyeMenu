#include "views/view.hpp"

namespace big
{
	inline void render_king_of_the_castle_ui()
	{
		components::sub_title("King Of The Castle");
		ImGui::PushID(3);
		components::button("Start Event", [] {
			if (scripts::force_host("am_king_of_the_castle"_J))
				if (auto script = gta_util::find_script_thread("am_king_of_the_castle"_J))
					*script_local(script->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx).at(1).at(1).as<int*>() = 0;
		});
		ImGui::SameLine();
		components::button("Finish Event", [] {
			if (scripts::force_host("am_king_of_the_castle"_J))
				if (auto script = gta_util::find_script_thread("am_king_of_the_castle"_J))
					*script_local(script->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx).at(1).at(3).as<int*>() = 0;
		});
		ImGui::PopID();

		components::button("Become The King", [] {
			if (scripts::force_host("am_king_of_the_castle"_J))
			{
				if (auto kotc = gta_util::find_script_thread("am_king_of_the_castle"_J))
				{
					auto kotc_king = script_local(kotc->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx).at(6).at(0, 204).at(74).at(0, 4);
					*kotc_king.as<int*>() = 0;
					*kotc_king.at(1).as<int*>() = self::id;
					*kotc_king.at(2).as<int*>() = self::id;
					*kotc_king.at(3).as<float*>() = 999999999.0f;
				}
			}
		});
		ImGui::SameLine();
		components::button("Dethrone Everyone", [] {
			if (scripts::force_host("am_king_of_the_castle"_J))
			{
				if (auto kotc = gta_util::find_script_thread("am_king_of_the_castle"_J))
				{
					for (int i = 0; i < *script_local(kotc->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx)
					                         .at(6)
					                         .at(0, 204)
					                         .at(74)
					                         .as<int*>();
					     i++)
					{
						*script_local(kotc->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx)
						     .at(6)
						     .at(0, 204)
						     .at(74)
						     .at(i, 4)
						     .as<int*>() = -1;
						*script_local(kotc->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx)
						     .at(6)
						     .at(0, 204)
						     .at(74)
						     .at(i, 4)
						     .at(1)
						     .as<int*>() = -1;
						*script_local(kotc->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx)
						     .at(6)
						     .at(0, 204)
						     .at(74)
						     .at(i, 4)
						     .at(2)
						     .as<int*>() = -1;
						*script_local(kotc->m_stack, scr_locals::am_king_of_the_castle::broadcast_idx)
						     .at(6)
						     .at(0, 204)
						     .at(74)
						     .at(i, 4)
						     .at(3)
						     .as<float*>() = -1.0f;
					}
				}
			}
		});
	}
}
