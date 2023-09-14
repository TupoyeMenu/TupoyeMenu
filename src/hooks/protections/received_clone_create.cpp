/**
 * @file received_clone_create.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "services/players/player_service.hpp"
#include "util/notify.hpp"

namespace big
{
	void hooks::received_clone_create(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, eNetObjType object_type, int32_t object_id, int32_t object_flag, rage::datBitBuffer* buffer, int32_t timestamp)
	{
		if (object_type < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || object_type > eNetObjType::NET_OBJ_TYPE_TRAIN)
		{
			notify::crash_blocked(src, "out of bounds object type");
			return;
		}

		auto plyr = g_player_service->get_by_id(src->m_player_id);

		if (plyr && plyr->block_clone_create)
			return;

		g.m_syncing_player      = src;
		g.m_syncing_object_type = object_type;

		g.debug.fuzzer.thread_id = GetCurrentThreadId();
		if (g.debug.fuzzer.enabled_object_types[(int)object_type])
			g.debug.fuzzer.active = true;
		g.debug.fuzzer.syncing_object_id = object_id;
		g_hooking->get_original<hooks::received_clone_create>()(mgr, src, dst, object_type, object_id, object_flag, buffer, timestamp);
		g.debug.fuzzer.active = false;
	}
}