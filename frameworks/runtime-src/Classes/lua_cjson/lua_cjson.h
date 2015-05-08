//
//  lua_cjson.h
//  Boids
//
//  Created by Xin Xu on 12/23/14.
//
//

#ifndef Boids_lua_cjson_h
#define Boids_lua_cjson_h

#define ENABLE_CJSON_GLOBAL 1

#include <lua.h>
#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef WIN32
#define snprintf _snprintf
#define strncasecmp strncmp
#define vsnprintf _vsnprintf
#endif

int luaopen_cjson(lua_State *l);
  
#ifdef __cplusplus
}
#endif

#endif
