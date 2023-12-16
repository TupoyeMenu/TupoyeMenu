#include "services/ped_animations/ped_animations_service.hpp"
#include "util/animations.hpp"
#include "views/view.hpp"

namespace big
{
	void view::animations()
	{
		ImGui::BeginGroup();

		static std::string category = "Default";
		static ped_animation deletion_ped_animation{};

		if (!std::string(deletion_ped_animation.name).empty())
			ImGui::OpenPopup("##deletepedanimation");

		if (ImGui::BeginPopupModal("##deletepedanimation", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Are you sure you want to delete %s?", deletion_ped_animation.name.c_str());

			ImGui::Spacing();

			if (ImGui::Button("Yes"))
			{
				g_ped_animation_service.delete_saved_animation(category, deletion_ped_animation);
				deletion_ped_animation.name = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				deletion_ped_animation.name = "";
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PushItemWidth(250);
		components::input_text_with_hint("##dict", "Dictionary", g_ped_animation_service.current_animation.dict);
		components::options_modal(
		    "Debug animations",
		    [] {
			    view::debug_animations(&g_ped_animation_service.current_animation.dict, &g_ped_animation_service.current_animation.anim);
		    },
		    true,
		    "List From Debug");
		components::input_text_with_hint("##anim", "Animation", g_ped_animation_service.current_animation.anim);

		ImGui::SameLine();
		components::button("Play", [] {
			g_ped_animation_service.play_saved_ped_animation(g_ped_animation_service.current_animation, self::ped);
		});
		ImGui::SameLine();
		components::button("Stop", [] {
			TASK::CLEAR_PED_TASKS(self::ped);
		});

		if (ImGui::TreeNode("Advanced Options"))
		{
			ImGui::SliderFloat("Blend in", &g_ped_animation_service.current_animation.blendin, -5, 10);
			ImGui::SliderFloat("Blend out", &g_ped_animation_service.current_animation.blendout, -5, 10);
			ImGui::InputInt("Duration in milliseconds", &g_ped_animation_service.current_animation.time_to_play);
			ImGui::SameLine();
			components::help_marker("-1 will make the duration indefinite, assuming it is looped");
			ImGui::PopItemWidth();

			ImGui::Checkbox("Ambient", &g_ped_animation_service.current_animation.ambient);
			ImGui::SameLine();
			components::help_marker("Current location and rotation will be saved and used");

			if (g_ped_animation_service.current_animation.ambient)
			{
				g_ped_animation_service.current_animation.pos[0] = self::pos.x;
				g_ped_animation_service.current_animation.pos[1] = self::pos.y;
				g_ped_animation_service.current_animation.pos[2] = self::pos.z;

				g_ped_animation_service.current_animation.rot[0] = self::rot.x;
				g_ped_animation_service.current_animation.rot[1] = self::rot.y;
				g_ped_animation_service.current_animation.rot[2] = self::rot.z;
			}
			else
			{
				g_ped_animation_service.current_animation.pos[0] = 0;
				g_ped_animation_service.current_animation.pos[1] = 0;
				g_ped_animation_service.current_animation.pos[2] = 0;

				g_ped_animation_service.current_animation.rot[0] = 0;
				g_ped_animation_service.current_animation.rot[1] = 0;
				g_ped_animation_service.current_animation.rot[2] = 0;
			}

			ImGui::BeginGroup(); //Regular flags

			ImGui::CheckboxFlags("Looped", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::LOOPING));
			ImGui::CheckboxFlags("Hold Last Frame", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::HOLD_LAST_FRAME));
			ImGui::CheckboxFlags("Uninterruptable", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::NOT_INTERRUPTABLE));
			ImGui::CheckboxFlags("Only Upperbody", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::UPPERBODY));
			ImGui::CheckboxFlags("Secondary", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::SECONDARY));
			ImGui::SameLine();
			components::help_marker("Allow primary animations to run simultaneously, such as walking.");
			ImGui::CheckboxFlags("Realize Animation Orientation", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::REORIENT_WHEN_FINISHED));
			ImGui::SameLine();
			components::help_marker("Use the final orientation achieved in the animation.");
			ImGui::CheckboxFlags("Hide Weapon", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::HIDE_WEAPON));

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			//Sync flags
			ImGui::CheckboxFlags("Sync In", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::TAG_SYNC_IN));
			ImGui::SameLine();
			components::help_marker("Seamless transition into the animation, for example from walking.");

			ImGui::CheckboxFlags("Sync Out", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::TAG_SYNC_OUT));
			ImGui::SameLine();
			components::help_marker("Seamless transition out of the animation, for example to continue walking.");
			ImGui::CheckboxFlags("Sync Continuous", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::TAG_SYNC_CONTINUOUS));
			ImGui::SameLine();
			components::help_marker("Seamless transition during the animation, especially usefull for upperbody animations.");
			ImGui::CheckboxFlags("Force Start", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::FORCE_START));
			ImGui::CheckboxFlags("Disable Collision", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::TURN_OFF_COLLISION));
			ImGui::CheckboxFlags("Override Physics", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::OVERRIDE_PHYSICS));
			ImGui::CheckboxFlags("Ignore Gravity", reinterpret_cast<unsigned int*>(&g_ped_animation_service.current_animation.flags), static_cast<unsigned int>(animations::anim_flags::IGNORE_GRAVITY));

			ImGui::EndGroup();

			ImGui::TreePop();
		}

		ImGui::SeparatorText("Save To File");

		components::input_text_with_hint("Category", "The category the animation will be saved under.", category);
		components::input_text_with_hint("Name", "File Name", g_ped_animation_service.current_animation.name);

		static auto save_response = [=]() -> bool {
			if (!STREAMING::DOES_ANIM_DICT_EXIST(g_ped_animation_service.current_animation.dict.data()))
			{
				g_notification_service->push_warning("Animations",
				    std::format("Dictionary with the name {} does not exist.", g_ped_animation_service.current_animation.dict));
				return false;
			}

			if (g_ped_animation_service.get_animation_by_name(g_ped_animation_service.current_animation.name))
			{
				g_notification_service->push_warning("Animations",
					std::format("Animation with the name {} already exists.", g_ped_animation_service.current_animation.name));
				return false;
			}

			if (category.empty())
			{
				g_notification_service->push_warning("Animations", "Category cannot be empty.");
				return false;
			}

			if (g_ped_animation_service.current_animation.anim.empty())
			{
				g_notification_service->push_warning("Animations", "Animation name cannot be empty.");
				return false;
			}

			return true;
		};

		ImGui::SameLine();

		components::button("Save", [] {
			if (save_response())
				g_ped_animation_service.save_new_animation(category, g_ped_animation_service.current_animation);
		});

		ImGui::EndGroup();

		ImGui::SeparatorText("Saved");
		components::button("Refresh", [] {
			g_ped_animation_service.fetch_saved_animations();
		});

		components::small_text("Double click to play.");
		components::small_text("Shift click to delete.");

		ImGui::SameLine();

		ImGui::Checkbox("Prompt Ambient", &g.self.prompt_ambient_animations);
		ImGui::SameLine();
		components::help_marker("Ambient animations will be prompted if you are close to one.");

		static std::string filter;

		ImGui::BeginGroup();

		components::input_text_with_hint("##filter", "Search", filter);

		ImGui::BeginGroup();
		components::small_text("Categories");
		if (ImGui::BeginListBox("##categories", {200, static_cast<float>(*g_pointers->m_gta.m_resolution_y * 0.4)}))
		{
			for (auto& l : g_ped_animation_service.all_saved_animations | std::ranges::views::keys)
			{
				if (ImGui::Selectable(l.data(), l == category))
				{
					category = l;
				}
			}
			ImGui::EndListBox();
		}
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		components::small_text("Animations");
		if (ImGui::BeginListBox("##animations", {200, static_cast<float>(*g_pointers->m_gta.m_resolution_y * 0.4)}))
		{
			if (g_ped_animation_service.all_saved_animations.find(category)
			    != g_ped_animation_service.all_saved_animations.end())
			{
				std::vector<ped_animation> current_list{};

				if (!filter.empty())
					current_list = g_ped_animation_service.saved_animations_filtered_list(filter);
				else
					current_list = g_ped_animation_service.all_saved_animations.at(category);

				for (const auto& p : current_list)
				{
					if (ImGui::Selectable(p.name.data(), p.name == g_ped_animation_service.current_animation.name, ImGuiSelectableFlags_AllowDoubleClick))
					{
						if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
						{
							deletion_ped_animation = p;
						}
						else
						{
							g_ped_animation_service.current_animation = p;
							if (ImGui::IsMouseDoubleClicked(0))
							{
								g_fiber_pool->queue_job([p] {
									g_ped_animation_service.play_saved_ped_animation(g_ped_animation_service.current_animation, self::ped);
								});
							}
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						if (p.name.length() > 25)
							ImGui::TextUnformatted(p.name.data());

						ImGui::Text("%s: %s\n%s: %s", "Dictionary", p.dict.c_str(), "Animation", p.anim.c_str());

						if (p.ambient)
							ImGui::BulletText("%s","Ambient Animation");
						ImGui::EndTooltip();
					}
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndGroup();

		ImGui::EndGroup();
	}
}