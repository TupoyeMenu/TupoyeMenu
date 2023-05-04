/**
 * @file check_chat_profanity.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "hooking.hpp"

namespace big
{
	enum class eProfanity
	{
		PROFANITY_CHAT_CLEAN = -1,
		PROFANITY_CLEAN,
		PROFANITY_DIRTY,
		PROFANITY_UNK,
		PROFANITY_ERROR
	};

	int hooks::check_chat_profanity(__int64 chat_type, const char* input, const char** output)
	{
		if (g.session.chat_force_clean)
		{
			return static_cast<int>(eProfanity::PROFANITY_CHAT_CLEAN);
		}
		return g_hooking->get_original<hooks::check_chat_profanity>()(chat_type, input, output);
	}
}
