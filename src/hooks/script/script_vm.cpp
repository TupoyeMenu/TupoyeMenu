/**
 * @file script_vm.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/scr_globals.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "gta_util.hpp"
#include "script_mgr.hpp"
#include "invoker.hpp"
#include "services/script_patcher/script_patcher_service.hpp"

#include <script/globals/GlobalPlayerBD.hpp>
#include <script/scrProgram.hpp>

static int old_cayo_flags;
static int old_shop_index;

namespace big
{
	class script_vm_guard
	{
		rage::scrProgram* m_program;
		uint8_t** m_orig_bytecode;

	public:
		script_vm_guard(rage::scrProgram* program) :
		    m_program(program)
		{
			m_orig_bytecode = program->m_code_blocks;

			if (auto bytecode = g_script_patcher_service->get_script_bytecode(program->m_name_hash))
				program->m_code_blocks = bytecode;

			if (g_pointers->m_gta.m_script_globals[0xA])
			{
				scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].CurrentShopIndex = old_shop_index;
				scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].CayoPericoFlags  = old_cayo_flags;
			}

			g.in_script_vm = true;
		}

		~script_vm_guard()
		{
			m_program->m_code_blocks = m_orig_bytecode;

			if (g_pointers->m_gta.m_script_globals[0xA])
			{
				old_shop_index = scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].CurrentShopIndex;
				old_cayo_flags = scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].CayoPericoFlags;

				if (g.spoofing.hide_from_player_list)
				{
					scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].CurrentShopIndex = -1;
					scr_globals::globalplayer_bd.as<GlobalPlayerBD*>()->Entries[self::id].CayoPericoFlags  = 1;
				}
			}

			g.in_script_vm = false;
		}
	};

	static uint32_t tmp_frame_count;
	rage::eThreadState hooks::script_vm(uint64_t* start_stack, uint64_t** scr_globals, rage::scrProgram* program, rage::scrThreadContext* ctx)
	{
		script_vm_guard guard(program);

		g_native_invoker.cache_handlers();
		if(g_running && ctx->m_state == rage::eThreadState::running && ctx->m_script_hash != RAGE_JOAAT("startup") && tmp_frame_count != *g_pointers->m_gta.m_frame_count)
		{
			tmp_frame_count = *g_pointers->m_gta.m_frame_count;
			gta_util::execute_as_script(ctx->m_script_hash, std::mem_fn(&script_mgr::tick), g_script_mgr);
		}

		return g_hooking->get_original<hooks::script_vm>()(start_stack, scr_globals, program, ctx);
	}
}
