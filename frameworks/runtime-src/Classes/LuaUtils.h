#ifndef _Boids_Lua_Utils_h_
#define _Boids_Lua_Utils_h_

#include <string>

class LuaUtils //Utils直接导不出，牵扯比较多；通过这么多一层来用
{
public:
	static void log(std::string content);
	static void logf(std::string content);
	static void logr(std::string content, bool only_save_to_replay = false); //不仅带帧号打出来，还会存到replay里用于比较（如果开了存snapshot的话）。打的帧号是replay里的

	static int randomNumber(int upper_bound); //返回[1, upper_bound]
	static float randomFloat(); //返回[0..1)之间浮点数

	static int randomNumber2(int upper_bound); //返回[1, upper_bound]，调试用
	static float randomFloat2(); //返回[0..1)之间浮点数，调试用

	static bool c();
};

#endif