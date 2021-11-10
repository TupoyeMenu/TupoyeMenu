#include "backend/looped/looped.hpp"
#include "core/enums.hpp"
#include "natives.hpp"


namespace big
{
	void looped::tunables_no_spam_help()
	{
		if(g.tunables.no_spam_help)
		{
			HUD::HIDE_HELP_TEXT_THIS_FRAME();
		}
	}
}