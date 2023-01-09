#include "object_spooner_service.hpp"
#include "renderer.hpp"
#include "gui.hpp"

namespace big
{
	object_spooner_service::object_spooner_service()
	{
		g_object_spooner_service = this;
		g_renderer->add_wndproc_callback([this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			wndproc(hwnd, msg, wparam, lparam);
		});
	}

	object_spooner_service::~object_spooner_service()
	{
		g_object_spooner_service = nullptr;
	}

	void object_spooner_service::draw_imgui()
	{
		if(g_gui->is_mouse_active())
			g_object_spooner_service->m_cursor.m_pos = ImGui::GetMousePos();
		else
			g_object_spooner_service->m_cursor.m_pos = {static_cast<float>(*g_pointers->m_resolution_x) / 2, static_cast<float>(*g_pointers->m_resolution_y) / 2};
		
		g_object_spooner_service->m_cursor.m_is_visible = g.object_spooner.enabled;

		if(g_object_spooner_service->m_cursor.m_is_visible)
		{
			g_object_spooner_service->draw_crosshair(
				g_object_spooner_service->m_cursor.m_is_active,
				ImGui::GetBackgroundDrawList()
			);
		}
	}

	void object_spooner_service::draw_crosshair(bool is_active, ImDrawList* dl)
	{
		if(is_active)
		{
			dl->AddRectFilled(
				ImVec2(m_cursor.m_pos.x + 15, m_cursor.m_pos.y + 0.8),
				ImVec2(m_cursor.m_pos.x - 15, m_cursor.m_pos.y - 0.8),
				IM_COL32(0, 255, 0, 255)
			);
			dl->AddRectFilled(
				ImVec2(m_cursor.m_pos.x + 0.8, m_cursor.m_pos.y + 15),
				ImVec2(m_cursor.m_pos.x - 0.8, m_cursor.m_pos.y - 15),
				IM_COL32(0, 255, 0, 255)
			);
		}
		else
		{
			dl->AddRectFilled(
				ImVec2(m_cursor.m_pos.x + 14, m_cursor.m_pos.y + 0.8),
				ImVec2(m_cursor.m_pos.x - 14, m_cursor.m_pos.y - 0.8),
				IM_COL32(255, 255, 255, 255)
			);
			dl->AddRectFilled(
				ImVec2(m_cursor.m_pos.x + 0.8, m_cursor.m_pos.y + 14),
				ImVec2(m_cursor.m_pos.x - 0.8, m_cursor.m_pos.y - 14),
				IM_COL32(255, 255, 255, 255)
			);
		}
	}

	static const ControllerInputs controls[] =
	{
		ControllerInputs::INPUT_LOOK_LR,
		ControllerInputs::INPUT_LOOK_UD,
		ControllerInputs::INPUT_LOOK_UP_ONLY,
		ControllerInputs::INPUT_LOOK_DOWN_ONLY,
		ControllerInputs::INPUT_LOOK_LEFT_ONLY,
		ControllerInputs::INPUT_LOOK_RIGHT_ONLY,
		ControllerInputs::INPUT_LOOK_LEFT,
		ControllerInputs::INPUT_LOOK_RIGHT,
		ControllerInputs::INPUT_LOOK_UP,
		ControllerInputs::INPUT_LOOK_DOWN,
	};

	void object_spooner_service::on_disable()
	{
		CAM::SET_CAM_ACTIVE(m_camera, false);
		CAM::RENDER_SCRIPT_CAMS(false, true, 500, true, true, 0);
		CAM::DESTROY_CAM(m_camera, false);
		STREAMING::CLEAR_FOCUS();
		ENTITY::FREEZE_ENTITY_POSITION(m_camera, false);
		g_gui->toggle(false, g_gui->is_mouse_active() ? true : false);
	}

	void object_spooner_service::on_enable()
	{
		m_camera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
		
		m_position = CAM::GET_GAMEPLAY_CAM_COORD();
		m_rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);

		CAM::SET_CAM_COORD(m_camera, m_position.x, m_position.y, m_position.z);
		CAM::SET_CAM_ROT(m_camera, m_rotation.x, m_rotation.y, m_rotation.z, 2);
		CAM::SET_CAM_ACTIVE(m_camera, true);
		CAM::RENDER_SCRIPT_CAMS(true, true, 500, true, true, 0);
	}

	void object_spooner_service::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if(wparam == g.settings.hotkeys.object_spooner_ctx)
		{
			switch (msg)
			{
				case WM_KEYDOWN:
				{
					if(g.object_spooner.enabled && !g_gui->is_mouse_active())
					{
						if(g_gui->is_open())
							g_gui->toggle_mouse(true);
						else			
							g_gui->toggle(true, true);
					}

					break;
				}
				case WM_KEYUP:
				{
					if(g.object_spooner.enabled && g_gui->is_mouse_active())
					{
						if(g_gui->is_open())
							g_gui->toggle_mouse(false);
						else			
							g_gui->toggle(false, true);
					}

					break;
				}
			}
		}
	}

	void object_spooner_service::spooner_tick()
	{
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

		if(!g_gui->is_mouse_active())
		{
			for (const auto& control : controls)
				PAD::ENABLE_CONTROL_ACTION(0, static_cast<int>(control), true);
		}

		Vector3 vecChange = { 0.f, 0.f, 0.f };

		// Left Shift
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_SPRINT))
			vecChange.z += speed / 2;
		// Left Control
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_DUCK))
			vecChange.z -= speed / 2;
		// Forward
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_UP_ONLY))
			vecChange.y += speed;
		// Backward
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_DOWN_ONLY))
			vecChange.y -= speed;
		// Left
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_LEFT_ONLY))
			vecChange.x -= speed;
		// Right
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_RIGHT_ONLY))
			vecChange.x += speed;

		if (vecChange.x == 0.f && vecChange.y == 0.f && vecChange.z == 0.f)
			mult = 0.f;
		else if (mult < 10)
			mult += 0.1f;

		Vector3 rot = CAM::GET_CAM_ROT(m_camera, 2);
		float pitch = math::deg_to_rad(rot.x); // vertical
		//float roll = rot.y;
		float yaw = math::deg_to_rad(rot.z); // horizontal

		m_position.x += (vecChange.x * cos(yaw) - vecChange.y * sin(yaw)) * mult;
		m_position.y += (vecChange.x * sin(yaw) + vecChange.y * cos(yaw)) * mult;
		m_position.z += vecChange.z * mult;

		CAM::SET_CAM_COORD(m_camera, m_position.x, m_position.y, m_position.z);
		STREAMING::SET_FOCUS_POS_AND_VEL(m_position.x, m_position.y, m_position.z, 0.f, 0.f, 0.f);

		m_rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
		CAM::SET_CAM_ROT(m_camera, m_rotation.x, m_rotation.y, m_rotation.z, 2);
		CAM::SET_THIRD_PERSON_CAM_RELATIVE_PITCH_LIMITS_THIS_UPDATE(-90, 90);

		if(g_gui->is_mouse_active())
		{
			g_object_spooner_service->m_should_use_sync_raycast = true;
			m_raycast.m_hit_something = entity::raycast_mouse(
				&m_raycast.m_hit_entity,
				&m_raycast.m_startCoords,
				&m_raycast.m_endCoords,
				&m_raycast.m_surfaceNormal,
				m_moving_entity.m_entity_handle
			);
			g_object_spooner_service->m_should_use_sync_raycast = false;
		}
		else
		{
			m_raycast.m_hit_something = entity::raycast(
				&m_raycast.m_hit_entity,
				&m_raycast.m_endCoords,
				&m_raycast.m_surfaceNormal,
				m_position,
				math::rotation_to_direction(m_rotation),
				m_moving_entity.m_entity_handle
			);
		}
		
		m_raycast.m_did_hit_entity = m_raycast.m_hit_something && ENTITY::DOES_ENTITY_EXIST(m_raycast.m_hit_entity) && ( ENTITY::IS_ENTITY_A_PED(m_raycast.m_hit_entity) || ENTITY::IS_ENTITY_A_VEHICLE(m_raycast.m_hit_entity) || ENTITY::IS_ENTITY_AN_OBJECT(m_raycast.m_hit_entity) );

		m_cursor.m_is_active = m_raycast.m_did_hit_entity || m_moving_entity.m_entity_handle;

		if(PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_ATTACK) && (m_raycast.m_did_hit_entity || m_moving_entity.m_entity_handle != 0))
		{
			if(m_moving_entity.m_entity_handle == 0)
				m_moving_entity.m_entity_handle = m_raycast.m_hit_entity;

			ENTITY::SET_ENTITY_COORDS(m_moving_entity.m_entity_handle, m_raycast.m_endCoords.x, m_raycast.m_endCoords.y, m_raycast.m_endCoords.z, false, true, true, false);
		}
		if(PAD::IS_DISABLED_CONTROL_RELEASED(0, (int)ControllerInputs::INPUT_ATTACK) && m_moving_entity.m_entity_handle != 0)
		{
			ENTITY::FREEZE_ENTITY_POSITION(m_moving_entity.m_entity_handle, false);
			m_moving_entity.m_entity_handle = 0;
		}

		if(PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_CONTEXT) && m_moving_entity.m_entity_handle != 0)
		{
			if(m_moving_entity.m_entity_handle == 0)
				m_moving_entity.m_entity_handle = m_raycast.m_hit_entity;

			Vector3 entity_rotation = ENTITY::GET_ENTITY_ROTATION(m_moving_entity.m_entity_handle, 2);
			ENTITY::SET_ENTITY_ROTATION(m_moving_entity.m_entity_handle, entity_rotation.x, entity_rotation.y, entity_rotation.z + 2.5, 2, false);
		}

		if(PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_CONTEXT_SECONDARY) && m_moving_entity.m_entity_handle != 0)
		{
			if(m_moving_entity.m_entity_handle == 0)
				m_moving_entity.m_entity_handle = m_raycast.m_hit_entity;

			Vector3 entity_rotation = ENTITY::GET_ENTITY_ROTATION(m_moving_entity.m_entity_handle, 2);
			ENTITY::SET_ENTITY_ROTATION(m_moving_entity.m_entity_handle, entity_rotation.x, entity_rotation.y, entity_rotation.z - 2.5, 2, false);
		}

		if(PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_CELLPHONE_OPTION) && m_raycast.m_did_hit_entity)
		{
			entity::delete_entity(m_raycast.m_hit_entity);
		}

		if(PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_AIM) && m_raycast.m_did_hit_entity)
		{
			selected_entity.m_does_entity_exist = ENTITY::DOES_ENTITY_EXIST(m_raycast.m_hit_entity);
			selected_entity.m_is_entity_a_ped = ENTITY::IS_ENTITY_A_PED(m_raycast.m_hit_entity);
			selected_entity.m_is_entity_a_vehicle = ENTITY::IS_ENTITY_A_VEHICLE(m_raycast.m_hit_entity);
			selected_entity.m_is_entity_an_object = ENTITY::IS_ENTITY_AN_OBJECT(m_raycast.m_hit_entity);
			selected_entity.m_entity_handle = m_raycast.m_hit_entity;
			selected_entity.m_entity = (CObject*)g_pointers->m_handle_to_ptr(m_raycast.m_hit_entity);
			g_gui->toggle(true, false);
		}

		if(PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_FRONTEND_CANCEL))
		{
			g_gui->toggle(false);
		}
	}

	void object_spooner_service::object_spooner()
	{
		while (g_running)
		{
			if(g.object_spooner.enabled && !g_object_spooner_service->m_last_active)
			{
				g_object_spooner_service->m_last_active = g.object_spooner.enabled;
				g_object_spooner_service->on_enable();
			}
			if(!g.object_spooner.enabled && g_object_spooner_service->m_last_active)
			{
				g_object_spooner_service->m_last_active = g.object_spooner.enabled;
				g_object_spooner_service->on_disable();
			}

			if (!g.object_spooner.enabled)
			{
				script::get_current()->yield();
				continue;
			}

			if (g.object_spooner.enabled)
			{
				g_object_spooner_service->spooner_tick();
			}

			script::get_current()->yield();
		}
	}
}