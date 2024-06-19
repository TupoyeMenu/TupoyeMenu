#include "backend/looped_command.hpp"
#include "gta/enums.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "hooking/hooking.hpp"
#include "util/base64.hpp"


namespace big
{
	class override_game_hashes : looped_command
	{
		using looped_command::looped_command;

		constexpr static int checksum_block_size = 0xF4;
		char original_checksum_block[checksum_block_size];
		char new_checksum_block[checksum_block_size];

		virtual void on_enable() override
		{
			memcpy(original_checksum_block, *g_pointers->m_gta.m_game_checksum_data, checksum_block_size);

			size_t output_len;
			auto decoded = base64::base64_decode(g.spoofing.game_checksum_data_b64.data(),
			    g.spoofing.game_checksum_data_b64.size(),
			    output_len);
			memcpy(new_checksum_block, decoded.get(), checksum_block_size);
		}

		virtual void on_tick() override
		{
			memcpy(*g_pointers->m_gta.m_game_checksum_data, new_checksum_block, checksum_block_size);
		}

		virtual void on_disable() override
		{
			memcpy(*g_pointers->m_gta.m_game_checksum_data, original_checksum_block, checksum_block_size);
		}
	};

	override_game_hashes g_override_game_hashes("overridegamehashes", "Override Game Hashes", "Allows you to load rpf mods and play in public online sessions", g.spoofing.override_game_hashes);
}
