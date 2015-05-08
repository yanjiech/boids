#ifndef __Boids_Map_Debugger_h_
#define __Boids_Map_Debugger_h_

#include "cocos2d.h"
#include <vector>

class MapDebugger
{
private:
	std::vector<cocos2d::Point> points;

public:
	void addPoint(cocos2d::Point pt)
	{
		points.push_back(pt);
	}

	void onTouchEvent(cocos2d::Point event_pt);
};

extern MapDebugger map_dbg;

#endif