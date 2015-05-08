//
//  lua_boids_manual.h
//  Boids
//
//  Created by Xin Xu on 11/18/14.
//
//

#ifndef __Boids__lua_boids_manual__
#define __Boids__lua_boids_manual__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_all_boids_manual(lua_State *L);

#endif /* defined(__Boids__lua_boids_manual__) */
