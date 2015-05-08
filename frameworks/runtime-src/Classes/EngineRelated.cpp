#include "EngineRelated.h"
#include "Net.h"
#include "CmdLineDebugger.h""
#include "Utils.h"

void battle_irrelevant_update(float dt)
{
	tcpUpdate(dt);

	std::string cmd = CmdLineDebugger::getInstance()->popCmd();
	if (cmd[0] == 's')
	{
		std::string replay_name = cmd.substr(2);
		Utils::saveReplay(replay_name);
	}
}