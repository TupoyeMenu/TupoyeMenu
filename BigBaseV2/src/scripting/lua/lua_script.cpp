#include "lua_script.hpp"
#include "lua_logger.hpp"
#include "lua_globals.hpp"
#include "lua_vector.hpp"
#include "lua_natives.hpp"
#include "script_mgr.hpp"
#include "lua_bindings.hpp"
#include "fiber_pool.hpp"

namespace big
{
	namespace lua::script
	{
		void add_script(luabridge::LuaRef func, const char* name, lua_State* state)
		{
			if (!func.isFunction())
				LOG(FATAL) << "ref not a function";

			g_script_mgr.add_script(std::make_unique<big::script>([state, func]
			{
				lua_script* script = luabridge::getGlobal(state, "!script").cast<lua_script*>();
				script->m_scripts.push_back(big::script::get_current());

				while (g_running)
				{
					try
					{
						func();
					}
					catch (luabridge::LuaException& ex)
					{
						LOG(INFO) << ex.what();
						return;
					}

					big::script::get_current()->yield();
				}
			}, name));
		}

		void add_callback(luabridge::LuaRef func, lua_State* state)
		{
			g_fiber_pool->queue_job([state, func]{
				lua_script* script = luabridge::getGlobal(state, "!script").cast<lua_script*>();
				script->m_scripts.push_back(big::script::get_current());

				try
				{
					func();
				}
				catch (luabridge::LuaException& ex)
				{
					LOG(INFO) << ex.what();
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

		void load(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("script")
					.addFunction("add_script", add_script)
					.addFunction("add_callback", add_callback)
					.addFunction("yield", yield)
					.addFunction("wait", wait)
				.endNamespace();
		}
	}

	lua_script::lua_script(std::filesystem::path path)
	{
		m_state = luaL_newstate();
		luaL_openlibs(m_state);
		lua::script::load(m_state);
		lua::logger::load(m_state);
		lua::globals::load(m_state);
		lua::vector::load(m_state);
		lua::natives::load(m_state);

		luabridge::setGlobal(m_state, this, "!script");

		if (luaL_dofile(m_state, path.string().c_str()))
			LOG(WARNING) << "Failed to load script " << path.filename().string();
		else
			LOG(INFO) << "Loaded script " << path.filename().string();
	}

	lua_script::~lua_script()
	{
		LOG(INFO) << "Script is unloading...";
		for (auto& script : m_scripts)
			g_script_mgr.remove_script(script);

		lua_close(m_state);
	}
}

