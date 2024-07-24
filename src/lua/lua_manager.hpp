#pragma once
#if defined (ENABLE_LUA)
#include "lua_module.hpp"
#endif // ENABLE_LUA

namespace big
{
	class lua_manager final
	{
#if defined (ENABLE_LUA)
	private:
		std::mutex m_module_lock;
		std::vector<std::shared_ptr<lua_module>> m_modules;
		std::mutex m_disabled_module_lock;
		std::vector<std::shared_ptr<lua_module>> m_disabled_modules;

		static constexpr std::chrono::seconds m_delay_between_changed_scripts_check = 3s;
		std::chrono::high_resolution_clock::time_point m_wake_time_changed_scripts_check;

		folder m_disabled_scripts_folder;
		folder m_scripts_folder;
		folder m_scripts_config_folder;

	private:
		void draw_tab(rage::joaat_t tab_hash, std::shared_ptr<big::lua_module> module);
		void draw_child_tabs(rage::joaat_t tab_hash, std::shared_ptr<big::lua_module> module, bool draw_tab_bar = false);

	public:
		lua_manager(folder scripts_folder, folder scripts_config_folder);
		~lua_manager();

		void disable_all_modules();
		void enable_all_modules();

		void load_all_modules();
		void unload_all_modules();

		inline auto get_module_count() const
		{
			return m_modules.size();
		}

		inline const folder& get_scripts_folder() const
		{
			return m_scripts_folder;
		}

		inline const folder& get_scripts_config_folder() const
		{
			return m_scripts_config_folder;
		}

		std::weak_ptr<lua_module> get_module(rage::joaat_t module_id);
		std::weak_ptr<lua_module> get_disabled_module(rage::joaat_t module_id);

		bool has_gui_to_draw(rage::joaat_t tab_hash);
		void draw_independent_gui();
		void draw_gui(rage::joaat_t tab_hash);
		void draw_child_tabs_external(rage::joaat_t tab_hash);
		void draw_tabs();

		bool dynamic_hook_pre_callbacks(const uintptr_t target_func_ptr, lua::memory::type_info_t return_type, lua::memory::runtime_func_t::return_value_t* return_value, std::vector<lua::memory::type_info_t> param_types, const lua::memory::runtime_func_t::parameters_t* params, const uint8_t param_count);
		void dynamic_hook_post_callbacks(const uintptr_t target_func_ptr, lua::memory::type_info_t return_type, lua::memory::runtime_func_t::return_value_t* return_value, std::vector<lua::memory::type_info_t> param_types, const lua::memory::runtime_func_t::parameters_t* params, const uint8_t param_count);

		std::weak_ptr<lua_module> enable_module(rage::joaat_t module_id);
		std::weak_ptr<lua_module> disable_module(rage::joaat_t module_id);

		void unload_module(rage::joaat_t module_id);
		std::weak_ptr<lua_module> load_module(const std::filesystem::path& module_path);

		void reload_changed_scripts();

		void handle_error(const sol::error& error, const sol::state_view& state);

		template<menu_event menu_event_, typename Return = void, typename... Args>
		inline std::conditional_t<std::is_void_v<Return>, void, std::optional<Return>> trigger_event(Args&&... args)
		{
			std::lock_guard guard(m_module_lock);

			for (auto& module : m_modules)
			{
				if (auto vec = module->m_event_callbacks.find(menu_event_); vec != module->m_event_callbacks.end())
				{
					for (auto& cb : vec->second)
					{
						auto result = cb(args...);

						if (!result.valid())
						{
							handle_error(result, result.lua_state());
							continue;
						}

#ifndef __GNUC__
						if constexpr (!std::is_void_v<Return>)
						{
							if (result.return_count() == 0)
								continue;

							if (!result[0].template is<Return>())
								continue;

							return result[0].template get<Return>();
						}
#endif // __GNUC__
					}
				}
			}

			if constexpr (!std::is_void_v<Return>)
				return std::nullopt;
		}

		inline void for_each_module(auto func)
		{
			std::lock_guard guard(m_module_lock);

			for (auto& module : m_modules)
			{
				func(module);
			}
		}

		inline void for_each_disabled_module(auto func)
		{
			std::lock_guard guard(m_disabled_module_lock);

			for (auto& module : m_disabled_modules)
			{
				func(module);
			}
		}
#else
		public:

			void draw_gui(rage::joaat_t) {} // Nullsub to allow compilation
#endif // ENABLE_LUA
	};

	inline lua_manager* g_lua_manager;
}
