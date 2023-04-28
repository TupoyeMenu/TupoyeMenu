#ifdef ENABLE_ASI_LOADER
#include "shv_runner.hpp"

#include "common.hpp"
#include "natives.hpp"
#include "script.hpp"

namespace big
{
	void shv_runner::run_tick()
	{
		for (auto func : scripts)
		{
			func.second();
		}
	}

	void shv_runner::script_func()
	{
		while (g_running)
		{
			run_tick();
			script::get_current()->yield();
		}
	}

	void shv_runner::shutdown()
	{
		for (auto& iter : big::shv_runner::scripts)
		{
			big::shv_runner::scripts.erase(iter.first);
			FreeLibrary(iter.first);
			CloseHandle(iter.first);
		}
		big::g.debug.asi_plugins_loaded = false;
	}
}
#endif // ENABLE_ASI_LOADER
