#ifndef __Boids_Net_for_Export_h_
#define __Boids_Net_for_Export_h_

#include <string>

//对应的实现在Net.cpp里

class Net
{
public:
	static void initialize(std::string server_ip, int server_port);

	static std::string getUserId();
};

#endif