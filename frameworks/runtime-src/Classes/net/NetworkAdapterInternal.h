#ifndef _Boids_Network_Adapter_internal_h_
#define _Boids_Network_Adapter_internal_h_

#include "base/NetworkInterface.h"
#include "pvp.pb.h"
#include "CCLuaEngine.h"

class NetworkAdaperInternal : public cocos2d::NetworkInterface
{
public:
	bool tryReceive();
};

void extendNetworkAdapter(lua_State *L);

#endif