#include "MapDebugger.h"

void MapDebugger::onTouchEvent(cocos2d::Point event_pt)
{
	for (const auto& pt : points)
	{
		if (pt.distanceSquared(event_pt) < 1000.0f)
		{
			cocos2d::log("MapDebugger: %.3f, %.3f (%.2f)", pt.x, pt.y, pt.distance(event_pt));
		}
	}
}

MapDebugger map_dbg;