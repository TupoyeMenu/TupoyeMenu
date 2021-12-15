#pragma once
#include "common.hpp"
#include "imgui.h"

namespace big
{
	class tab_player
	{
	public:
		static void tab_info();
		static void tab_teleport();
		static void tab_toxic();
		static void tab_player_weapon();
		static void tab_player_explosions();
	};
}