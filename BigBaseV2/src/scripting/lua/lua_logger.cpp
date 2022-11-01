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

	void load(sol::state& state)
	{
		auto ns = state["logger"].get_or_create<sol::table>();
		ns["info"] = info;
		ns["debug"] = debug;
		ns["warning"] = warning;
		ns["fatal"] = fatal;
	}
}