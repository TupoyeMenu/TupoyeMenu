#pragma once
#include "core/scr_globals.hpp"

namespace big::outfit
{
	struct outfit_t
	{
		int id;
		std::string label;
		int drawable_id     = 0;
		int drawable_id_max = 0;
		int texture_id      = 0;
		int texture_id_max  = 0;
	};

	struct components_t
	{
		std::vector<outfit_t> items = {{0, "HEAD"},
		    {1, "BERD"},
		    {2, "HAIR"},
		    {3, "UPPR"},
		    {4, "LOWR"},
		    {5, "HAND"},
		    {6, "FEET"},
		    {7, "TEEF"},
		    {8, "ACCS"},
		    {9, "TASK"},
		    {10, "DECL"},
		    {11, "JBIB"}};
	};

	struct props_t
	{
		std::vector<outfit_t> items = {{0, "HEAD"},
		    {1, "GLASSES"},
		    {2, "EARS"},
		    {3, "UNK1"},
		    {4, "UNK2"},
		    {5, "UNK3"},
		    {6, "WATCH"},
		    {7, "WRIST"},
		    {8, "UNK4"}};
	};

	inline void check_bounds_drawable(outfit_t* item)
	{
		if(item->drawable_id > item->drawable_id_max)
			item->drawable_id = item->drawable_id_max;
		if(item->drawable_id < 0)
			item->drawable_id = 0;
	}

	inline void check_bounds_texture(outfit_t* item)
	{
		if(item->texture_id > item->texture_id_max)
			item->texture_id = item->texture_id_max;
		if(item->texture_id < 0)
			item->texture_id = 0;
	}

	// usually each update increases 1//
	inline char* get_slot_name_address(int slot)
	{
		return scr_globals::stats.at(0, 5568).at(681).at(2462).at(slot, 8).as<char*>();
	}

	inline int* get_component_drawable_id_address(int slot, int id)
	{
		return scr_globals::stats.at(0, 5568).at(681).at(1338).at(slot, 13).at(id, 1).as<int*>();
	}

	inline int* get_component_texture_id_address(int slot, int id)
	{
		return scr_globals::stats.at(0, 5568).at(681).at(1612).at(slot, 13).at(id, 1).as<int*>();
	}

	inline int* get_prop_drawable_id_address(int slot, int id)
	{
		return scr_globals::stats.at(0, 5568).at(681).at(1886).at(slot, 10).at(id, 1).as<int*>();
	}

	inline int* get_prop_texture_id_address(int slot, int id)
	{
		return scr_globals::stats.at(0, 5568).at(681).at(2097).at(slot, 10).at(id, 1).as<int*>();
	}
}