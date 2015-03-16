#ifndef _Boids_Network_Interface_h_
#define _Boids_Network_Interface_h_

namespace cocos2d
{
	class NetworkInterface
	{
	public:
		virtual bool tryReceive() = 0;
	};
}

#endif