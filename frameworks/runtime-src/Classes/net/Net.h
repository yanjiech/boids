#ifndef __Boids_Net_h_
#define __Boids_Net_h_

#include "tolua_fix.h"
#include "CCLuaEngine.h"

void tcpUpdate(float dt); //每一帧都调一下

void extendNet(lua_State *L);

#endif