#include "backend/looped/looped.hpp"
#include "core/enums.hpp"
#include "natives.hpp"


namespace big
{
	void looped::tunables_BLOCK_JOIN_REQUESTS()
	{
		if (g.tunables.BLOCK_JOIN_REQUESTS)
		{
			NETWORK::NETWORK_SESSION_BLOCK_JOIN_REQUESTS(g.tunables.BLOCK_JOIN_REQUESTS);
		}
	}
}