/**
 * @file notify.hpp
 * @brief Basic notification related functions.
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "gta/enums.hpp"
#include "natives.hpp"
#include "network/CNetGamePlayer.hpp"
#include "network/ChatData.hpp"
#include "pointers.hpp"
#include "script.hpp"

#include <script/HudColor.hpp>

namespace big::notify
{
	/**
	 * @brief Shows a notification above minimap.
	 * 
	 * @param text Notification text.
	 */
	inline void above_map(std::string_view text)
	{
		HUD::SET_TEXT_OUTLINE();
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.data());
		HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
	}

	/**
	 * @brief Processes crash messages.
	 * Shows a YimMenu error notification with player and crash name;
	 * Logs (WARNING) with player and crash name;
	 * Adds `TRAIED_CRASH_PLAYER` infration to player.
	 * 
	 * @param player Player that is trying to crash us, can be null.
	 * @param crash Crash name to display.
	 */
	inline void crash_blocked(CNetGamePlayer* player, const char* crash)
	{
		if (player)
		{
			g_notification_service->push_error("Protections", std::format("Blocked {} crash from {}", crash, player->get_name()));
			LOG(WARNING) << std::format("Blocked {} crash from {}({})",
			    crash,
			    player->get_name(),
			    player->get_net_data() ? player->get_net_data()->m_gamer_handle.m_rockstar_id : 0);
		}
		else
		{
			g_notification_service->push_error("Protections", std::format("Blocked {} crash from unknown player", crash));
			LOG(WARNING) << std::format("Blocked {} crash from unknown player", crash);
		}
	}

	/**
	 * @brief Shows a busy spinner till the value at the address equals the value passed or if timeout is hit
	 */
	inline void busy_spinner(std::string_view text, int* address, int value, int timeout = 15)
	{
		HUD::BEGIN_TEXT_COMMAND_BUSYSPINNER_ON("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.data());
		HUD::END_TEXT_COMMAND_BUSYSPINNER_ON(3);

		for (size_t i = 0; *address != value && i < (size_t)timeout * 100; i++)
			script::get_current()->yield(10ms);

		HUD::BUSYSPINNER_OFF();
	}

	/**
	 * @brief Shows subtitle with given text.
	 *
	 * @param text Text to display.
	 * @param ms Time in milliseconds to show text on screen before disappearing.
	 */
	inline void show_subtitle(std::string_view text, int ms = 2000)
	{
		HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.data());
		HUD::END_TEXT_COMMAND_PRINT(ms, 1);
	}

	/**
	 * @brief Shows help text using natives.
	 * 
	 * @param text Text to display.
	 */
	inline void display_help_text(std::string_view text)
	{
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.data());
		HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, 1, -1);
	}

	/**
	 * @brief Shows `{} joined.` message above map.
	 * 
	 * @param net_game_player Player to get the name from.
	 */
	inline void player_joined(CNetGamePlayer* net_game_player)
	{
		above_map(std::format("<C>{}</C> joined.", net_game_player->get_name()));
	}

	inline void draw_chat(char* msg, const char* player_name, bool is_team)
	{
		int scaleform = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MULTIPLAYER_CHAT");

		while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(scaleform))
			script::get_current()->yield();

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(scaleform, "ADD_MESSAGE");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(player_name); // player name
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(msg);             // content
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(is_team ? "MP_CHAT_TEAM" : "MP_CHAT_ALL")); // scope
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);                               // teamOnly
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT((int)HudColor::HUD_COLOUR_PURE_WHITE); // eHudColour
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(scaleform, "SET_FOCUS");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);                                    // VISIBLE_STATE_DEFAULT
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);                                    // scopeType (unused)
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);                                    // scope (unused)
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(player_name);           // player
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT((int)HudColor::HUD_COLOUR_PURE_WHITE); // eHudColour
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleform, 255, 255, 255, 255, 0);

		// fix broken scaleforms, when chat alrdy opened
		if (const auto chat_data = *g_pointers->m_gta.m_chat_data; chat_data && (chat_data->m_chat_open || chat_data->m_timer_two))
			HUD::CLOSE_MP_TEXT_CHAT();
	}
}