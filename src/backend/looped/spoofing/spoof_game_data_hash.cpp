#include "backend/looped_command.hpp"
#include "pointers.hpp"
#include <game_files/GameDataHash.hpp>

namespace
{
	static std::array<std::uint32_t, 15> orig_hash;

	void store_data(std::array<std::uint32_t, 15>& data)
	{
		for (int i = 0; i < 15; i++)
		{
			data[i] = (*big::g_pointers->m_gta.m_game_data_hash)->m_data[i];
		}
	}

	void load_data(const std::array<std::uint32_t, 15>& data)
	{
		for (int i = 0; i < 15; i++)
		{
			(*big::g_pointers->m_gta.m_game_data_hash)->m_data[i] = data[i];
		}
	}
}

namespace big
{
	class spoof_game_data_hash : looped_command
	{
		using looped_command::looped_command;

		virtual void on_enable() override
		{
			store_data(orig_hash);
		}

		virtual void on_tick() override
		{
			if (g.spoofing.game_data_hash_dirty)
			{
				load_data(g.spoofing.game_data_hash);
				g.spoofing.game_data_hash_dirty = false;
			}
		}

		virtual void on_disable() override
		{
			load_data(orig_hash);
		}
	};

	class copy_current_game_data_hash : command
	{
		using command::command;

		virtual void execute(const command_arguments&, const std::shared_ptr<command_context> ctx) override
		{
			if (g.spoofing.spoof_game_data_hash)
				g.spoofing.game_data_hash = orig_hash;
			else
				store_data(g.spoofing.game_data_hash);

			g.spoofing.game_data_hash_dirty = true;
		}
	};

	spoof_game_data_hash g_spoof_game_data_hash("spoofdatahash", "Spoof Data Hashes", "Spoofs the game data hashes. This can be used to join sessions with custom DLCs loaded", g.spoofing.spoof_game_data_hash);
	copy_current_game_data_hash g_copy_current_game_data_hash("storecurrenthash", "Copy Current Data Hash", "Copies the active game hash into the input box. Do this before loading custom DLC files", 0);
}
