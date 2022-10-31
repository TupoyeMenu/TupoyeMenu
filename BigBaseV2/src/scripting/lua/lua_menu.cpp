#include "lua_menu.hpp"
#include "services/players/player_service.hpp"
#include "pointers.hpp"

namespace big::lua::menu
{
	void show_message(const char* title, const char* msg)
	{
		g_notification_service->push(title, msg);
	}

	void show_warning(const char* title, const char* msg)
	{
		g_notification_service->push_warning(title, msg);
	}

	void show_error(const char* title, const char* msg)
	{
		g_notification_service->push_error(title, msg);
	}

	int get_selected_player()
	{
		if (g_player_service->get_selected()->is_valid())
			return g_player_service->get_selected()->id();

		return 255;
	}

	void trigger_script_event(sol::variadic_args args)
	{
		if (args.size() < 2)
			return;

		int bitset = args[0].get<int>();
		auto data = new std::int64_t[args.size() - 1];

		for (int i = 1; i < args.size(); i++)
			data[i - 1] = args[i].get<int>();

		if (args.size() > 3)
			data[1] = self::id;

		g_pointers->m_trigger_script_event(1, data, (int)(args.size() - 1), bitset);

		delete[] data;
	}

	void load(sol::state& state)
	{
		auto ns = state["menu"].get_or_create<sol::table>();
		ns["show_message"] = show_message;
		ns["show_warning"] = show_warning;
		ns["show_error"] = show_error;
		ns["get_selected_player"] = get_selected_player;
		ns["trigger_script_event"] = trigger_script_event;
	}
}