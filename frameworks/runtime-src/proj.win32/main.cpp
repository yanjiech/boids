#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include "CmdLineDebugger.h"
#include "Utils.h"
#include "ReplayComparer.h"

USING_NS_CC;

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
	freopen("debug.log", "w", stdout);
	freopen("err.log", "w", stderr);
    //freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);

	CmdLineDebugger::getInstance()->listen(); //不知道mac下能不能用；有需求再说
#endif

	Utils::runTest();

    // create the application instance
    AppDelegate app;

	if (std::string(lpCmdLine, 2) == "-t")
	{
		char r1[100], r2[100];
		sscanf(lpCmdLine + 2, " %s%s", r1, r2);
		ReplayComparer rc;
		rc.compare(r1, r2);
	}

    int ret = Application::getInstance()->run();

#ifdef USE_WIN32_CONSOLE
	CmdLineDebugger::getInstance()->stopListen();

    if (!ret)
    {
        system("pause");
    }
    FreeConsole();
#endif

    return ret;
}
