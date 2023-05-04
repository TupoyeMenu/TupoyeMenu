/**
 * @file animator.hpp
 * 
 * @deprecated Animations from YimMenu UI.
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

namespace big
{
	class animator
	{
		std::chrono::time_point<std::chrono::system_clock> animation_start_time;
		std::chrono::time_point<std::chrono::system_clock> animation_current_time;
		float animation_curve(float t)
		{
			return 3 * pow(1.f - t, 2.f) * t + 3 * (1.f - t) * pow(t, 2.f) + pow(t, 3.f);
		}

	public:
		void animate(int milliseconds, std::function<void(float&)> cb)
		{
			animation_current_time = std::chrono::system_clock::now();
			float timeDiff = (float)std::chrono::duration_cast<std::chrono::milliseconds>(animation_current_time - animation_start_time)
			                     .count();
			float t = timeDiff / milliseconds;

			if (t > 1)
			{
				t = 1;
			}

			float progress = animation_curve(t);
			cb(progress);
		}

		void reset()
		{
			animation_start_time = std::chrono::system_clock::now();
		}
	};
}