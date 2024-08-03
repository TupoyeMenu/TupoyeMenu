/**
 * @file carmod_shop.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "native_hooks.hpp"
#include "natives.hpp"

namespace big
{
	namespace carmod_shop
	{
		inline void SET_ENTITY_COORDS(rage::scrNativeCallContext* src)
		{
			if (!g.vehicle.ls_customs)
			{
				ENTITY::SET_ENTITY_COORDS(src->get_arg<Entity>(0), src->get_arg<float>(1), src->get_arg<float>(2), src->get_arg<float>(3), src->get_arg<BOOL>(4), src->get_arg<BOOL>(5), src->get_arg<BOOL>(6), src->get_arg<BOOL>(7));
			}
		}

		inline void SET_ENTITY_HEADING(rage::scrNativeCallContext* src)
		{
			if (!g.vehicle.ls_customs)
			{
				ENTITY::SET_ENTITY_HEADING(src->get_arg<Entity>(0), src->get_arg<float>(1));
			}
		}

		inline void SET_VEHICLE_LIGHTS(rage::scrNativeCallContext* src)
		{
			if (!g.vehicle.ls_customs)
			{
				VEHICLE::SET_VEHICLE_LIGHTS(src->get_arg<Vehicle>(0), src->get_arg<int>(1));
			}
		}

		inline void DISABLE_ALL_CONTROL_ACTIONS(rage::scrNativeCallContext* src)
		{
			if (!g.vehicle.ls_customs)
			{
				PAD::DISABLE_ALL_CONTROL_ACTIONS(src->get_arg<int>(0));
			}
		}
	}
}
