#pragma once

namespace big::lua::imgui
{
	class lua_component
	{
	public:
	    constexpr lua_component() noexcept
		{
		}

		virtual void draw() {};
	};
}