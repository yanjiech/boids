#ifndef _CmdLine_Debugger_h_
#define _CmdLine_Debugger_h_

#include <deque>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

class CmdLineDebugger
{
private:
	std::thread t;
	bool keep_listening;
	std::deque<std::string> cmd_queue;
	std::mutex queue_mutex;

	void run()
	{
		while (keep_listening)
		{
			std::string cmd;
			std::getline(std::cin, cmd);

			queue_mutex.lock();
			cmd_queue.push_back(cmd);
			queue_mutex.unlock();
		}
	}

	static class CmdLineDebugger* _instance;

public:
	CmdLineDebugger() {}

	void listen()
	{
		keep_listening = true;
		t = std::thread(&CmdLineDebugger::run, this);
	}

	void stopListen()
	{
		keep_listening = false;
		t.join(); //≤ªµ˜’‚––µƒª∞ª·°∞R6010 - abort() has been called°±
	}

	std::string popCmd()
	{
		std::string ret;
		queue_mutex.lock();
		if (cmd_queue.size())
		{
			ret = cmd_queue.front();
			cmd_queue.pop_front();
		}
		queue_mutex.unlock();
		return ret;
	}

	static CmdLineDebugger* getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new CmdLineDebugger();
		}
		return _instance;
	}
};

#endif