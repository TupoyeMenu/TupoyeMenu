/**
 * @file serialize_join_request_message.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta_util.hpp"
#include "hooking.hpp"

#include <network/Network.hpp>
#include <network/RemoteGamerInfoMsg.hpp>

namespace big
{
	bool hooks::serialize_join_request_message(RemoteGamerInfoMsg* info, void* data, int size, int* bits_serialized)
	{
		if (info->unk_0xC0 == 0)
			info->unk_0xC0 = 1;

		info->m_num_handles = 0;
		return g_hooking->get_original<hooks::serialize_join_request_message>()(info, data, size, bits_serialized);
	}
}