#pragma once
#include "base/HashTable.hpp"
#include "function_types.hpp"
#include "gta/fwddec.hpp"
#include "gta_pointers.hpp"
#include "memory/module.hpp"
#include "socialclub/ScInfo.hpp"
#include "services/gta_data/cache_file.hpp"
#include "util/compile_time_helpers.hpp"
#include "gta_pointers.hpp"
#ifdef ENABLE_SOCIALCLUB
	#include "sc_pointers.hpp"
#endif // ENABLE_SOCIALCLUB

namespace big
{
	class pointers
	{
	private:
		static void get_gta_batch(memory::module region);
		static void get_sc_batch(memory::module region);

	public:
		explicit pointers();
		~pointers();

	public:
		HWND m_hwnd{};

		gta_pointers m_gta;

#ifdef ENABLE_SOCIALCLUB
		socialclub_pointers m_sc;
#endif // ENABLE_SOCIALCLUB		
	};

	inline pointers* g_pointers{};
}
