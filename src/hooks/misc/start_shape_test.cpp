#include "hooking.hpp"
#include "services/object_spooner/object_spooner_service.hpp"

namespace big
{
    // Why not just make it a function argument R*????
	int hooks::start_shape_test(int64_t a1, bool _is_synchronous)
	{
        if(g_object_spooner_service->m_should_use_sync_raycast)
		    return g_hooking->get_original<start_shape_test>()(a1, true);
        else
            return g_hooking->get_original<start_shape_test>()(a1, _is_synchronous);
	}
}

