#include "GrassMonitor.h"

void GrassMonitor::clear()
{
	for (auto p : polygons)
	{
		delete p;
	}
	rtree.clear();
}

void GrassMonitor::add(BoidsPolygon* grass_poly)
{
	polygons.push_back(grass_poly);

	rtree.insert(std::make_pair(grass_poly->getBoundingBox(), grass_poly));
}

void GrassMonitor::query(cocos2d::Point pt, std::vector<std::string>& grass_stepped_on)
{
	for (rtree_poly::const_query_iterator it = rtree.qbegin(bgi::intersects(point(pt.x, pt.y))); it != rtree.qend(); ++it)
	{
		if (it->second->coversPoint(pt))
		{
			grass_stepped_on.push_back(it->second->name);
		}
	}
}