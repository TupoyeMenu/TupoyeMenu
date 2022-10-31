#include "lua_script.hpp"
#include "lua_imgui.hpp"
#include "imgui/lua_button.hpp"
#include "imgui/lua_input_float.hpp"
#include "imgui/lua_input_int.hpp"
#include "imgui/lua_input_text.hpp"
#include "imgui/lua_text.hpp"
#include "imgui/lua_separator.hpp"
#include "imgui/lua_sameline.hpp"
#include "imgui/lua_checkbox.hpp"

namespace big::lua::imgui
{
	void add_component(lua_State* state, std::uint32_t hash, std::shared_ptr<lua_component> component)
	{
		sol::state_view view(state);
		lua_script* script = view["!script"];
		script->m_script_gui[hash].push_back(std::move(component));
	}

	class lua_imgui_submenu
	{
		std::uint32_t m_hash;
	public:
		lua_imgui_submenu(const char* name) :
			m_hash(rage::joaat(name))
		{
		}

		std::shared_ptr<lua_button> add_button(const char* name, sol::function callback, sol::this_state state)
		{
			auto component = std::make_shared<lua_button>(name, callback);
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_text> add_text(const char* value, sol::this_state state)
		{
			auto component = std::make_shared<lua_text>(value);
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_input_float> add_input_float(const char* text, float default_val, sol::this_state state)
		{
			auto component = std::make_shared<lua_input_float>(text, default_val);
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_input_int> add_input_int(const char* text, int default_val, sol::this_state state)
		{
			auto component = std::make_shared<lua_input_int>(text, default_val);
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_input_text> add_input_text(const char* text, const char* default_val, sol::this_state state)
		{
			auto component = std::make_shared<lua_input_text>(text, default_val);
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_checkbox> add_checkbox(const char* text, bool default_val, sol::this_state state)
		{
			auto component = std::make_shared<lua_checkbox>(text, default_val);
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_separator> add_separator(sol::this_state state)
		{
			auto component = std::make_shared<lua_separator>();
			add_component(state, m_hash, component);
			return component;
		}

		std::shared_ptr<lua_sameline> add_sameline(sol::this_state state)
		{
			auto component = std::make_shared<lua_sameline>();
			add_component(state, m_hash, component);
			return component;
		}
	};

	lua_imgui_submenu get_submenu(const char* name)
	{
		return lua_imgui_submenu(name);
	}

	void load(sol::state& state)
	{
		auto ns = state["imgui"].get_or_create<sol::table>();

		auto button_type = ns.new_usertype<lua_button>("button");
		button_type["set_text"] = &lua_button::set_text;

		auto text_type = ns.new_usertype<lua_text>("text");
		text_type["set_text"] = &lua_text::set_text;
		text_type["set_font"] = &lua_text::set_font;

		auto input_float_type = ns.new_usertype<lua_input_float>("input_float");
		input_float_type["set_text"] = &lua_input_float::set_text;
		input_float_type["get_value"] = &lua_input_float::get_value;
		input_float_type["set_value"] = &lua_input_float::set_value;

		auto input_int_type = ns.new_usertype<lua_input_int>("input_int");
		input_int_type["set_text"] = &lua_input_int::set_text;
		input_int_type["get_value"] = &lua_input_int::get_value;
		input_int_type["set_value"] = &lua_input_int::set_value;

		auto input_text_type = ns.new_usertype<lua_input_text>("input_text");
		input_text_type["set_text"] = &lua_input_text::set_text;
		input_text_type["get_value"] = &lua_input_text::get_value;
		input_text_type["set_value"] = &lua_input_text::set_value;

		auto checkbox_type = ns.new_usertype<lua_checkbox>("checkbox");
		checkbox_type["set_text"] = &lua_checkbox::set_text;
		checkbox_type["is_enabled"] = &lua_checkbox::is_enabled;
		checkbox_type["set_enabled"] = &lua_checkbox::set_enabled;

		auto separator_type = ns.new_usertype<lua_separator>("separator");
		auto sameline_type = ns.new_usertype<lua_sameline>("sameline");

		auto submenu_type = ns.new_usertype<lua_imgui_submenu>("lua_imgui_submenu");
		submenu_type["add_button"] = &lua_imgui_submenu::add_button;
		submenu_type["add_text"] = &lua_imgui_submenu::add_text;
		submenu_type["add_input_int"] = &lua_imgui_submenu::add_input_int;
		submenu_type["add_input_float"] = &lua_imgui_submenu::add_input_float;
		submenu_type["add_input_text"] = &lua_imgui_submenu::add_input_text;
		submenu_type["add_checkbox"] = &lua_imgui_submenu::add_checkbox;
		submenu_type["add_separator"] = &lua_imgui_submenu::add_separator;
		submenu_type["add_sameline"] = &lua_imgui_submenu::add_sameline;

		ns["get_submenu"] = &get_submenu;
	}
}