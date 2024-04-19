#include "backend/bool_command.hpp"

namespace big
{
	bool_command g_graceful_landing("gracefullanding", "Graceful Landing", "No falling animation, removes slow falling, and always land on feet", g.self.graceful_landing);
}