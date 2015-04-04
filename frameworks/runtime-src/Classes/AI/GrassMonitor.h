#ifndef __Boids_Grass_Monitor_h
#define __Boids_Grass_Monitor_h

#include "BoidsPolygon.h"
#include "BoostGeometryDef.h"

typedef std::pair<box, class BoidsPolygon*> rtree_poly_value;
typedef bgi::rtree< rtree_poly_value, bgi::rstar<16, 4> > rtree_poly;

class GrassMonitor
{
protected:
	rtree_poly rtree;
	std::vector<class BoidsPolygon*> polygons;

public:
	void clear();

	void add(BoidsPolygon* grass_poly);

	void query(cocos2d::Point pt, std::vector<std::string>& grass_stepped_on);
};

#endif