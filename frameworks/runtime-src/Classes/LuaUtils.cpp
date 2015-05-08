#include "LuaUtils.h"
#include "cocos2d.h"
#include "Utils.h"

void LuaUtils::log(std::string content)
{
	cocos2d::log("%s", content.c_str());
}

void LuaUtils::logf(std::string content)
{
	Utils::logAI("%s", content.c_str());
}

void LuaUtils::logr(std::string content, bool only_save_to_replay)
{
	Utils::logr(content, only_save_to_replay);
}

int LuaUtils::randomNumber(int upper_bound)
{
	return Utils::randomNumber(upper_bound);
}

float LuaUtils::randomFloat()
{
	return Utils::randomFloat();
}

int LuaUtils::randomNumber2(int upper_bound)
{
	return Utils::randomNumber2(upper_bound);
}

float LuaUtils::randomFloat2()
{
	return Utils::randomFloat2();
}

bool LuaUtils::c()
{
	logf("--c--");
	return true;
}