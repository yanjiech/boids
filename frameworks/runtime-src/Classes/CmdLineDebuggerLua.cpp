#include "CmdLineDebuggerLua.h"
#include "CmdLineDebugger.h"

std::string CmdLineDebuggerLua::popCmd()
{
	return CmdLineDebugger::getInstance()->popCmd();
}