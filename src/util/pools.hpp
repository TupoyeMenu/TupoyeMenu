/**
 * @file pools.hpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "gta/pools.hpp"

namespace big::pools
{
	inline auto& get_all_peds()
	{
		return **g_pointers->m_gta.m_ped_pool;
	}

	inline auto& get_all_vehicles()
	{
		return ***g_pointers->m_gta.m_vehicle_pool;
	}

	inline auto& get_all_props()
	{
		return **g_pointers->m_gta.m_prop_pool;
	}

	inline auto get_all_peds_array()
	{
		return get_all_peds().to_array();
	}

	inline auto get_all_vehicles_array()
	{
		return get_all_vehicles().to_array();
	}

	inline auto get_all_props_array()
	{
		return get_all_props().to_array();
	}
};