#pragma once
#include "natives.hpp"
#include "fiber_pool.hpp"
#include "gta/joaat.hpp"
#include "gta/enums.hpp"
#include "gta/fwddec.hpp"
#include "util/math.hpp"
#include "util/entity.hpp"
#include <base/CObject.hpp>
#include "imgui.h"

namespace big
{
	struct selected_entity
	{
		Entity m_entity_handle;
		CObject* m_entity;
		bool m_does_entity_exist = false;
		bool m_is_entity_a_ped = false;
		bool m_is_entity_a_vehicle = false;
		bool m_is_entity_an_object = false;
	};

	struct raycast
	{
		bool m_hit_something;
		Entity m_hit_entity = 0;
		Vector3 m_endCoords;
		Vector3 m_startCoords;
		Vector3 m_surfaceNormal;
		bool m_did_hit_entity = false;
	};

	struct cursor
	{
		bool m_is_visible = false;
		bool m_is_active = false;
		ImVec2 m_pos;
	};

	class object_spooner_service final
	{
	private:
		Cam m_camera = -1;
		Vector3 m_position;
		Vector3 m_rotation;

		raycast m_raycast;
		cursor m_cursor;
		selected_entity m_moving_entity;

		bool m_last_active;

		void on_enable();
		void on_disable();
		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		void draw_crosshair(bool is_active, ImDrawList* dl);
		void spooner_tick();
	public:
		object_spooner_service();
		~object_spooner_service();

		object_spooner_service(const object_spooner_service&) = delete;
		object_spooner_service(object_spooner_service&&) noexcept = delete;
		object_spooner_service& operator=(const object_spooner_service&) = delete;
		object_spooner_service& operator=(object_spooner_service&&) noexcept = delete;

		static void draw_imgui();
		static void object_spooner();

		float speed = 0.5f;
		float mult = 0.f;

		selected_entity selected_entity;

		bool m_should_use_sync_raycast = false;
	};

	inline object_spooner_service* g_object_spooner_service;
}