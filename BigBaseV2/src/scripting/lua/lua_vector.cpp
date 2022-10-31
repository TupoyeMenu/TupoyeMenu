#include "lua_vector.hpp"
#include "natives.hpp"

namespace big::lua::vector
{
	void load(sol::state& state)
	{
		auto vector_type = state.new_usertype<Vector3>("vector", sol::constructors<Vector3(float, float, float)>());
		vector_type["x"] = &Vector3::x;
		vector_type["y"] = &Vector3::y;
		vector_type["z"] = &Vector3::z;
	}
}