/**
 * @file prepare_metric_for_sending.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "hooking.hpp"
#include "rage/rlMetric.hpp"

namespace big
{
#pragma pack(push, 1)
	class json_serializer
	{
		uint32_t unk0;  // 0x00
		uint32_t unk1;  // 0x00
		char* buffer;   // 0x08
		uint32_t curlen;// 0x10
		uint32_t maxlen;// 0x14
		uint32_t unk4;  // 0x18
		uint8_t flags;  // 0x1C

	public:
		json_serializer(char* _buffer, uint32_t _length) :
		    buffer(_buffer),
		    maxlen(_length)
		{
			unk0
				= 0;
				unk1   = 0;
				curlen = 0;
				unk4   = 1;
				flags  = 0;
		}

		inline char* get_string() const
		{
			return buffer;
		}
	};
	static_assert(sizeof(json_serializer) == 0x1D);// size is actually 0x20
#pragma pack(pop)

	void hooks::prepare_metric_for_sending(rage::datBitBuffer* bit_buffer, int unk, int time, rage::rlMetric* metric)
	{
		if (g.debug.logs.metric_logs)
		{
			char buffer[256]{};
			json_serializer serializer(buffer, sizeof(buffer));

			metric->serialize(&serializer);

			LOG(INFO) << "METRIC: " << metric->get_name() << "; DATA: " << serializer.get_string();
		}
	}
}
