#include "hooking.hpp"
#include "native_hooks/native_hooks.hpp"

namespace big
{
	rage::eThreadState hooks::gta_thread_kill(GtaThread* thread)
	{
		const auto result = g_hooking->get_original<gta_thread_kill>()(thread);

		if (g->notifications.gta_thread_kill.log)
			LOG(INFO) << "Script Thread '" << thread->m_name << "' terminated.";
		if (g->notifications.gta_thread_kill.notify)
			g_notification_service->push("Script Thread Termination", std::format("Script Thread '{}' terminated.", thread->m_name));

		return result;
	}
}
