#include "lua_vector.hpp"
#include "natives.hpp"

namespace big::lua::vector
{
	void load(lua_State* state)
	{
		luabridge::getGlobalNamespace(state)
			.beginClass<Vector3>("vector")
				.addConstructor <void (*) (float, float, float)>()
				.addProperty("x", &Vector3::x)
				.addProperty("y", &Vector3::y)
				.addProperty("z", &Vector3::z);
	}
}