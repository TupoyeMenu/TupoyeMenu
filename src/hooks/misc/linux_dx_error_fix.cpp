#include "hooking.hpp"
#include "services/custom_text/custom_text_service.hpp"

namespace big
{
	int hooks::linux_dx_error_fix()
	{
		/*
		* 1000 - DX 10
		* 1010 - DX 10.1
		* 1100 - DX 11
		*/
		return 1100;
	}
}
