#ifndef __Boids_Path__
#define __Boids_Path__

#include <vector>
#include "cocos2d.h"

class Path
{
public:
    int validity_frame;
	std::vector<cocos2d::Point> steps;

	void paint(cocos2d::Point from, cocos2d::DrawNode* node, cocos2d::Color4F c);

	Path(const std::string& steps_serialization); //只会对steps进行赋值，主要是为了便于调试
	std::string serialize();
	bool operator == (const class Path& other); //只判断steps。主要是为了便于调试
    const Path& operator = ( const Path& other );

	cocos2d::Point walkFrame(cocos2d::Point current_pos, int max_walk_length); //返回的是新位置

	bool isOutdated(); //是否过期（需要重新寻路）
	bool isPathEnd(); //是否已经走完了预订的步骤。走完了的话这个Path实例就可以删掉了

	std::function<bool(cocos2d::Point pos)> customized_is_reach_destination;
	std::function<void()> path_released_callback;

	bool can_reach_final_destination;

    Path();
	Path(int _validity_frame);

	virtual ~Path();
};

#endif