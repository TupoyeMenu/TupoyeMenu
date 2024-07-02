#pragma once

namespace big
{
	class byte_patch_manager
	{
	public:
		byte_patch_manager();
		~byte_patch_manager();
	};

	inline byte_patch_manager* g_byte_patch_manager;
}
