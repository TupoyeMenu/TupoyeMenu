/**
 * @file creator.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "core/scr_globals.hpp"
#include "fiber_pool.hpp"
#include "native_hooks.hpp"
#include "natives.hpp"
#include "util/scripts.hpp"

namespace big
{
	namespace creator
	{
		void GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(rage::scrNativeCallContext* src)
		{
			const auto hash = src->get_arg<rage::joaat_t>(0);

			if (hash == RAGE_JOAAT("freemode") || hash == RAGE_JOAAT("main"))
			{
				src->set_return_value(0);
				return;
			}

			src->set_return_value(SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(hash));
		}

		void GET_ENTITY_MODEL(rage::scrNativeCallContext* src)
		{
			Entity entity = src->get_arg<Entity>(0);
			Hash model    = ENTITY::GET_ENTITY_MODEL(entity);

			if (entity == self::ped)
			{
				if (model != RAGE_JOAAT("mp_m_freemode_01") && model != RAGE_JOAAT("mp_f_freemode_01"))
					model = RAGE_JOAAT("mp_m_freemode_01");
			}

			src->set_return_value<Hash>(std::move(model));
		}

		void GET_USED_CREATOR_BUDGET(rage::scrNativeCallContext* src)
		{
			if (g.ugc.infinite_model_memory)
				src->set_return_value<float>(0);
			else
				src->set_return_value<float>(STREAMING::GET_USED_CREATOR_BUDGET());
		}
	}
}