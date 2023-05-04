/**
 * @file script_manager.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of TupoyeMenu.
 * TupoyeMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * TupoyeMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with TupoyeMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef ENABLE_ASI_LOADER
#include "script_manager.hpp"

#include "invoker.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script.hpp"
#include "shv_runner.hpp"
#include "util/pools.hpp"

#include <set>

#define DLL_EXPORT __declspec(dllexport)

void DLL_EXPORT scriptWait(unsigned long wait_time)
{
	typedef std::chrono::duration<unsigned long long> my_duran_duran_duration;
	big::script::get_current()->yield(std::chrono::duration_cast<my_duran_duran_duration>(std::chrono::milliseconds(wait_time)));
}

void DLL_EXPORT scriptRegister(HMODULE module, void (*function)())
{
	big::shv_runner::scripts.emplace(module, function);
}

void DLL_EXPORT scriptUnregister(void (*function)())
{
	for (auto iter : big::shv_runner::scripts)
		if (function == iter.second)
			big::shv_runner::scripts.erase(iter.first);
}

void DLL_EXPORT scriptUnregister(HMODULE module)
{
	big::shv_runner::scripts.erase(module);
}

eGameVersion DLL_EXPORT getGameVersion()
{
	return VER_1_0_2845_0;
}

void DLL_EXPORT scriptRegisterAdditionalThread(HMODULE module, void (*function)())
{
	big::shv_runner::scripts.emplace(module, function);
}

static uint64_t g_hash;

void DLL_EXPORT nativeInit(uint64_t hash)
{
	big::g_native_invoker.begin_call();
	if (hash == 0x43A66C31C68491C0)
		hash = 0x50FAC3A3E030A6E1;
	if (hash == 0x239528EACDC3E7DE)
		hash = 0x3882114BDE571AD4;
	g_hash = hash;
}

void DLL_EXPORT nativePush64(uint64_t value)
{
	big::g_native_invoker.push_arg(std::forward<uint64_t>(value));
}

PUINT64 DLL_EXPORT nativeCall()
{
	big::g_native_invoker.end_call(g_hash);
	return reinterpret_cast<uint64_t*>(big::g_native_invoker.get_return_address());
}

typedef void (*TKeyboardFn)(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

static std::set<TKeyboardFn> g_keyboardFunctions;

void DLL_EXPORT keyboardHandlerRegister(TKeyboardFn function)
{
	g_keyboardFunctions.insert(function);
}

void DLL_EXPORT keyboardHandlerUnregister(TKeyboardFn function)
{
	g_keyboardFunctions.erase(function);
}

void script_manager::wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP)
	{
		auto keyboard_functions = g_keyboardFunctions;

		for (auto& keyboard_function : keyboard_functions)
		{
			keyboard_function((DWORD)wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFF, (lParam >> 24) & 1, (uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP), (lParam >> 30) & 1, (uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP));
		}
	}
}

PUINT64 DLL_EXPORT getGlobalPtr(int global_id)
{
	return reinterpret_cast<PUINT64>(&big::g_pointers->m_gta.m_script_globals[global_id >> 18 & 0x3F][global_id & 0x3FFFF]);
}

int DLL_EXPORT worldGetAllPeds(int* arr, int array_size)
{
	return big::pools::get_all_peds().to_int_array(arr, array_size);
}

int DLL_EXPORT worldGetAllVehicles(int* arr, int array_size)
{
	return big::pools::get_all_vehicles().to_int_array(arr, array_size);
}

int DLL_EXPORT worldGetAllObjects(int* arr, int array_size)
{
	return big::pools::get_all_props().to_int_array(arr, array_size);
}

int DLL_EXPORT worldGetAllPickups(int* arr, int array_size)
{
	return big::pools::get_all_pickups().to_int_array(arr, array_size);
}

int DLL_EXPORT worldGetAllCameras(int* arr, int array_size)
{
	return big::pools::get_all_cameras().to_int_array(arr, array_size);
}

DLL_EXPORT BYTE* getScriptHandleBaseAddress(int handle)
{
	return (BYTE*)big::g_pointers->m_gta.m_handle_to_ptr(handle);
}

int DLL_EXPORT registerRawStreamingFile(const std::string& file_name, const std::string& register_as)
{
	int textureID = -1;
	if (big::g_pointers->m_gta.m_register_file(&textureID, file_name.c_str(), true, file_name.c_str(), false))
	{
		LOG(VERBOSE) << "Registered File " << file_name.c_str() << " with ID:" << textureID;
		return textureID;
	}

	LOG(FATAL) << "Failed to register " << file_name.c_str();
	return 0;
}

/* D3d SwapChain */
DLL_EXPORT void presentCallbackRegister(PresentCallback cb)
{
	big::g_renderer->add_callback(cb);
}

DLL_EXPORT void presentCallbackUnregister(PresentCallback cb)
{
	big::g_renderer->remove_callback(cb);
}

/* textures */
DLL_EXPORT int createTexture(const char* texfile_name)
{
	return -1; // Implement later if we can get the DirectXTK working.
}

DLL_EXPORT void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a)
{
	// Implement later if we can get the DirectXTK working.
}
#endif // ENABLE_ASI_LOADER
