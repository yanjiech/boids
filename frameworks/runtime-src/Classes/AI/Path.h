#ifndef __Boids_Path__
#define __Boids_Path__

#include <vector>
#include "cocos2d.h"

class Path : public cocos2d::Ref
{
public:
    int validity_frame;
	std::vector<cocos2d::Point> steps;
    
public:
	void paint(cocos2d::Point from, cocos2d::DrawNode* node, cocos2d::Color4F c);

	Path(const std::string& steps_serialization); //只会对steps进行赋值，主要是为了便于调试
	std::string serialize();
	bool operator == (const class Path& other); //只判断steps。主要是为了便于调试
//    const Path& operator = ( const Path& other );

	bool isOutdated();
	bool isPathEnd();

	std::function<bool(cocos2d::Point pos)> customized_is_reach_destination;
	std::function<void()> path_released_callback;

	bool can_reach_final_destination;

    Path();
	virtual ~Path();
    
    static Path* create( int valid_frame );
    virtual bool init( int valid_frame );
};

#endif