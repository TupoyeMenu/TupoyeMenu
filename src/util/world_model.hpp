/**
 * @file world_model.hpp
 * @brief Basic object related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"

namespace big::world_model
{
	constexpr size_t patch_size = 24;
	static inline std::once_flag once_flag;
	static inline std::array<byte, patch_size> backup;
	static inline void setup_backup()
	{
		memcpy(backup.data(), g_pointers->m_gta.m_world_model_spawn_bypass, patch_size);
	}

	/**
	 * @brief Spawns objects.
	 * @note Uses a bypass to allow spawning world models.
	 * 
	 * @param hash Model hash to spawn.
	 * @param location Location to spawn object on.
	 * @param is_networked Is this object synced to other players.
	 * @return Spawned object.
	 */
	inline Object spawn(Hash hash, Vector3 location = Vector3(), bool is_networked = true)
	{
		STREAMING::REQUEST_MODEL(hash);
		for (int i = 0; i < 100 && !STREAMING::HAS_MODEL_LOADED(hash); i++)
		{
			script::get_current()->yield();
		}
		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			LOG(WARNING) << "Failed to load model " << HEX_TO_UPPER(hash);
			return 0;
		}

		std::call_once(once_flag, setup_backup);
		memset(g_pointers->m_gta.m_world_model_spawn_bypass, 0x90, patch_size);

		const auto object = OBJECT::CREATE_OBJECT(hash, location.x, location.y, location.z, is_networked, false, false);

		memcpy(g_pointers->m_gta.m_world_model_spawn_bypass, backup.data(), patch_size);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		return object;
	}
}
