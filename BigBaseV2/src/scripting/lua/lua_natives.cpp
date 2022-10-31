#include "lua_natives.hpp"
#include "natives.hpp"
#include "invoker.hpp"
#include "lua_nativesgen.hpp"
#include "file_manager.hpp"

namespace big::lua::natives
{
	template<typename T> 
	T invoke(sol::variadic_args args)
	{
		g_native_invoker.begin_call();

		for (int i = 1; i < args.size(); i++)
		{
			if (args[i].is<int>())
				g_native_invoker.push_arg<int>(args[i].get<int>());
			if (args[i].is<float>())
				g_native_invoker.push_arg<float>(args[i].get<float>());
			else if (args[i].is<bool>())
				g_native_invoker.push_arg<bool>(args[i].get<bool>());
			else if (args[i].is<Any*>())
				g_native_invoker.push_arg<Any*>(args[i].get<Any*>());
			else if (args[i].is<const char*>())
				g_native_invoker.push_arg<const char*>(args[i].get<const char*>());
			else
			{
				LOG(WARNING) << "Unhandled parameter type";
				return T();
			}
		}

		g_native_invoker.end_call(args[0].as<std::uint64_t>());

		if constexpr (!std::is_void_v<T>)
			return g_native_invoker.get_return_value<T>();
	}

	static std::string read_all_lines(const std::filesystem::path path)
	{
		std::ifstream file(path);
		std::stringstream stream;
		stream << file.rdbuf();
		return stream.str();
	}

	void load(sol::state& state)
	{
		auto ns = state["_invoker"].get_or_create<sol::table>();
		ns["invoke_void"] = invoke<void>;
		ns["invoke_int"] = invoke<int>;
		ns["invoke_bool"] = invoke<bool>;
		ns["invoke_float"] = invoke<float>;
		ns["invoke_ptr"] = invoke<Any*>;
		ns["invoke_str"] = invoke<const char*>;
		ns["invoke_vec3"] = invoke<Vector3>;

		std::string natives;

		if (std::filesystem::exists(g_file_manager->get_project_file("./scripts/lua/natives.lua").get_path()))
			natives = read_all_lines(g_file_manager->get_project_file("./scripts/lua/natives.lua").get_path());
		else if (std::filesystem::exists("natives.lua"))
			natives = read_all_lines("natives.lua");
		else
			natives = lua_natives;

		state.load_buffer(natives.c_str(), natives.length())();
	}
}