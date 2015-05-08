#ifndef _Boids_Network_Adapter_h_
#define _Boids_Network_Adapter_h_

#include <string>

class NetworkAdapter
{
public:
	bool init(std::string host_ip, int host_port); //该方法现在是c++这边调用的

	void start();
	void stop();

	void setMove(int dir_x, int dir_y);
	void setSkill(int unit_id, int dir_x, int dir_y, int range);

	static NetworkAdapter* getInstance()
	{
		if (!_instance)
		{
			_instance = new NetworkAdapter();
		}
		return _instance;
	}

private:
	NetworkAdapter();
	static NetworkAdapter* _instance;
};

#endif