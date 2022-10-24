#include "common.hpp"
#include "lua_logger.hpp"

namespace big::lua::logger
{
	void info(const std::string& str)
	{
		LOG(INFO) << str;
	}

	void debug(const std::string& str)
	{
		LOG(G3LOG_DEBUG) << str;
	}

	void warning(const std::string& str)
	{
		LOG(WARNING) << str;
	}

	void fatal(const std::string& str)
	{
		LOG(FATAL) << str;
	}

	void load(lua_State* state)
	{
		luabridge::getGlobalNamespace(state)
			.beginNamespace("logger")
				.addFunction("info", info)
				.addFunction("debug", debug)
				.addFunction("warning", warning)
				.addFunction("fatal", fatal)
			.endNamespace();
	}
}