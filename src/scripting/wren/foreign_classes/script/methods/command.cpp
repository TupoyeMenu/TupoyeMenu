#include "scripting/wren/foreign_classes/script/wren_script.hpp"
#include "backend/command.hpp"
#include "backend/context/console_command_context.hpp"

namespace big
{
    void wren_script::command(WrenVM* vm)
    {
        LOG(INFO) << wrenGetSlotString(vm, 1);
		command::process(wrenGetSlotString(vm, 1), std::make_shared<console_command_context>());
    }
}
