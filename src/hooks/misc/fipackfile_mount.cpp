/**
 * @file fipackfile_mount.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gta/fidevice.hpp"
#include "hooking.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "services/gta_data/yim_fipackfile.hpp"

namespace big
{
	bool hooks::fipackfile_mount(rage::fiPackfile* this_, const char* mount_point)
	{
		static bool init = ([] {
			if (g_gta_data_service->state() == eGtaDataUpdateState::ON_INIT_WAITING)
				g_gta_data_service->set_state(eGtaDataUpdateState::ON_INIT_UPDATE_START);
		}(),true);

		auto result = g_hooking->get_original<fipackfile_mount>()(this_, mount_point);

		if (g_gta_data_service->state() == eGtaDataUpdateState::ON_INIT_UPDATE_START)
		{
			yim_fipackfile rpf_wrapper = yim_fipackfile(this_, mount_point);
			std::for_each(yim_fipackfile::m_wrapper_call_back.begin(), yim_fipackfile::m_wrapper_call_back.end(), [&rpf_wrapper](std::function<size_t(yim_fipackfile & rpf_wrapper)> cb) {
				cb(rpf_wrapper);
			});

			if (!stricmp(this_->GetName(), "BgScript.rpf"))
			{
				yim_fipackfile::for_each_fipackfile();
				g_gta_data_service->set_state(eGtaDataUpdateState::ON_INIT_UPDATE_END);
			}
		}

		return result;
	}
}
