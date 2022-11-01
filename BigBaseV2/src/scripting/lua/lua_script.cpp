#include "lua_script.hpp"
#include "lua_logger.hpp"
#include "lua_globals.hpp"
#include "lua_vector.hpp"
#include "lua_natives.hpp"
#include "lua_memory.hpp"
#include "lua_bindings.hpp"
#include "lua_manager.hpp"
#include "lua_imgui.hpp"
#include "lua_menu.hpp"

#include "fiber_pool.hpp"
#include "script_mgr.hpp"

namespace big
{
	namespace lua::script
	{
		void add_script(sol::function func, const char* name, sol::this_state state)
		{
			g_script_mgr.add_script(std::make_unique<big::script>([state, func]
			{
				sol::state_view view(state);
				lua_script* script = view["!script"];
				script->m_scripts.push_back(big::script::get_current());

				while (g_running)
				{
					try
					{
						func();
					}
					catch (const std::exception&)
					{
						return;
					}

					big::script::get_current()->yield();
				}
			}, name));
		}

		void add_callback(sol::function func)
		{
			g_fiber_pool->queue_job([func]{
				try
				{
					func();
				}
				catch (const std::exception&)
				{
					return;
				}
			});
		}

		void yield()
		{
			big::script::get_current()->yield();
		}

		void wait(int millis)
		{
			big::script::get_current()->yield(std::chrono::milliseconds(millis));
		}

		void load(sol::state& state)
		{
			auto ns = state["script"].get_or_create<sol::table>();
			ns["add_script"] = add_script;
			ns["add_callback"] = add_callback;
			ns["yield"] = yield;
			ns["wait"] = wait;
		}
	}

	int lua_error_handler(lua_State* state)
	{
		size_t messagesize;
		const char* message = lua_tolstring(state, -1, &messagesize);
		if (message)
		{
			std::string err(message, messagesize);
			LOG(WARNING) << "Error executing script: " << err;
		}

		throw std::exception();
	}

	lua_script::lua_script(std::filesystem::path path)
	{
		m_state = sol::state(lua_error_handler);
		luaL_openlibs(m_state);
		lua::script::load(m_state);
		lua::logger::load(m_state);
		lua::globals::load(m_state);
		lua::vector::load(m_state);
		lua::natives::load(m_state);
		lua::memory::load(m_state);
		lua::imgui::load(m_state);
		lua::menu::load(m_state);

		m_state["joaat"] = rage::joaat;
		m_state["!script"] = this;

		m_state.load_file(path.string().c_str())();
	}

	lua_script::~lua_script()
	{
		LOG(INFO) << "Script is unloading...";
		for (auto& script : m_scripts)
			g_script_mgr.remove_script(script);

		m_scripts.clear();
		m_script_gui.clear();
	}
}

