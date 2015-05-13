//
//  LuaFunc.cpp
//  Boids
//
//  Created by Xin Xu on 11/3/14.
//
//

#include "LuaFunc.h"
#include "CCLuaEngine.h"

using namespace cocos2d;

void LuaFunc::runBattleSceneWithMapPath(const std::string& mapPath) {
    auto stack = LuaEngine::getInstance()->getLuaStack();
    auto state = stack->getLuaState();
    lua_getglobal(state, "G");
    lua_pushstring(state, "runBattleSceneWithMapPath");
    lua_gettable(state, -2);
    if (!lua_isfunction(state, -1)) {
        CCLOG("runBattleSceneWithMapPath function is not found");
    } else {
        lua_pushstring(state, mapPath.c_str());
        lua_call(state, 1, 0);
    }
    lua_pop(state, 1); // pop G
}