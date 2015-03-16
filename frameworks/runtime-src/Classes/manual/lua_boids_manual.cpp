//
//  lua_boids_manual.cpp
//  Boids
//
//  Created by Xin Xu on 11/18/14.
//
//

#include "lua_boids_manual.h"
#include "tolua_fix.h"
#include "CCLuaEngine.h"
#include "EditMode.h"
#include "NetworkAdapterInternal.h"
#include "Net.h"
#include "ArgCheck.h"

using namespace cocos2d;

int lua_boids_EditMode_startMain(lua_State *L) {
    if (nullptr == L) {
        return 0;
    }
    int argc = 0;
    EditMode *self = nullptr;
    self = static_cast<EditMode *>(tolua_tousertype(L, 1, 0));
    argc = lua_gettop(L) - 1;
    if (1 == argc) {
        int exitHandler = toluafix_ref_function(L, 2, 0);
        self->enterMain([exitHandler](){
            if (exitHandler != 0) {
                LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(exitHandler, 0);
            }
        });
    } else {
        CCLOG("'startParticleSystem' function of EffectSprite has wrong number of arguments: %d, was expecting %d\n", argc, 4);
    }
    return 0;
}

void extendEditMode(lua_State *L) {
    tolua_beginmodule(L, "EditMode");
    tolua_function(L, "enterMain", lua_boids_EditMode_startMain);
    tolua_endmodule(L);
}

int register_all_boids_manual(lua_State *L) {
    if (nullptr == L) {
        return 0;
    }
    tolua_beginmodule(L, "ue");
    extendEditMode(L);
	extendNetworkAdapter(L);
	extendNet(L);
    tolua_endmodule(L);
    return 0;
}
