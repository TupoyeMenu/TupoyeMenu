#include "hooking/hooking.hpp"

namespace big
{
	bool hooks::receive_pickup(rage::netObject* object, void* unk, CPed* ped)
	{
		if (g.protections.receive_pickup)
		{
			g_notification_service->push_error("Protections", "Blocked pickup");
			return false;
		}

		return g_hooking->get_original<hooks::receive_pickup>()(object, unk, ped);
	}
}