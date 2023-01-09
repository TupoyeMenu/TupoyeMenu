#include "backend/command.hpp"
#include "backend/bool_command.hpp"

namespace big
{
	bool_command g_object_spooner("object_spooner", "Object Spooner", "Toggle object spooner", g.object_spooner.enabled);
}