#include "backend/bool_command.hpp"

namespace big
{
	bool_command g_enable_mk1_variants("enablemk1variants", "Enable MK1 and MK2 Weapons", "This will disable the game from removing the MK1 variant weapon from your inventory if you have the MK2 variant as well.
Use Ammunation on this page to add the MK1 variant.", g.weapons.enable_mk1_variants);
}