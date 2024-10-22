﻿#include "common.hpp"
#include "features.hpp"
#include "fiber_pool.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "thread_pool.hpp"
#include "shv_runner.h"
#include "ASI Loader/ASILoader.h"

#include "native_hooks/native_hooks.hpp"
#include "services/globals_service.hpp"
#include "services/player_service.hpp"
#include "services/mobile_service.hpp"
#include "services/vehicle_service.hpp"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD
		{
			while (!FindWindow(L"grcWindow", L"Grand Theft Auto V"))
				std::this_thread::sleep_for(1s);

			auto logger_instance = std::make_unique<logger>();
			try
			{
				LOG(RAW_GREEN_TO_CONSOLE) << "Yim's Menu Initializing";
				auto pointers_instance = std::make_unique<pointers>();
				LOG(INFO) << "Pointers initialized.";

				auto renderer_instance = std::make_unique<renderer>();
				LOG(INFO) << "Renderer initialized.";

				auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
				LOG(INFO) << "Fiber pool initialized.";

				auto hooking_instance = std::make_unique<hooking>();
				LOG(INFO) << "Hooking initialized.";

				g.load();
				LOG(INFO) << "Settings Loaded.";

				auto thread_pool_instance = std::make_unique<thread_pool>();
				LOG(INFO) << "Thread pool initialized.";

				auto globals_service_instace = std::make_unique<globals_service>();
				auto mobile_service_instance = std::make_unique<mobile_service>();
				auto player_service_instance = std::make_unique<player_service>();
				auto vehicle_service_instance = std::make_unique<vehicle_service>();
				LOG(INFO) << "Registered service instances...";

				g_script_mgr.add_script(std::make_unique<script>(&features::script_func));
				g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));
				g_script_mgr.add_script(std::make_unique<script>(&shv_runner::script_func));
				LOG(INFO) << "Scripts registered.";

				auto native_hooks_instance = std::make_unique<native_hooks>();
				LOG(INFO) << "Dynamic native hooker initialized.";

				g_hooking->enable();
				LOG(INFO) << "Hooking enabled.";

				ASILoader::Initialize();


				while (g_running)
				{
					std::this_thread::sleep_for(500ms);
				}

				g_hooking->disable();
				LOG(INFO) << "Hooking disabled.";

				std::this_thread::sleep_for(1000ms);

				native_hooks_instance.reset();
				LOG(INFO) << "Dynamic native hooker uninitialized.";

				g_script_mgr.remove_all_scripts();
				LOG(INFO) << "Scripts unregistered.";

				vehicle_service_instance.reset();
				mobile_service_instance.reset();
				player_service_instance.reset();
				globals_service_instace.reset();
				LOG(INFO) << "Services uninitialized.";

				// Make sure that all threads created don't have any blocking loops
				// otherwise make sure that they have stopped executing
				g_thread_pool->destroy();
				LOG(INFO) << "Destroyed thread pool.";

				thread_pool_instance.reset();
				LOG(INFO) << "Thread pool uninitialized.";

				hooking_instance.reset();
				LOG(INFO) << "Hooking uninitialized.";

				fiber_pool_instance.reset();
				LOG(INFO) << "Fiber pool uninitialized.";

				renderer_instance.reset();
				LOG(INFO) << "Renderer uninitialized.";

				pointers_instance.reset();
				LOG(INFO) << "Pointers uninitialized.";

				shv_runner::shutdown();
				LOG(INFO) << "ASI plugins unloaded.";

				vehicle_service_instance.reset();
			}
			catch (std::exception const &ex)
			{
				LOG(WARNING) << ex.what();
				MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
			}

			LOG(INFO) << "Farewell!";
			logger_instance.reset();

			CloseHandle(g_main_thread);
			FreeLibraryAndExitThread(g_hmodule, 0);
		}, nullptr, 0, &g_main_thread_id);
	}

	return true;
}
