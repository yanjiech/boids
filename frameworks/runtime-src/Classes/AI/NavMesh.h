#ifndef __Boids_Mesh__
#define __Boids_Mesh__

#include "BoidsPolygon.h"
#include "BoidsPolygonEx.h"
#include "BoostGeometryDef.h"
#include <queue>
#include "Path.h"
#include <memory>
#include "Collidable.h"

typedef std::pair<box, class BoidsPolygonEx*> rtree_poly_ex_value;
typedef bgi::rtree< rtree_poly_ex_value, bgi::rstar<16, 4> > rtree_poly_ex;

typedef std::pair<segment, class Border*> rtree_border_value;
typedef bgi::rtree< rtree_border_value, bgi::rstar<16, 4> > rtree_border;

class NavMesh
{
protected:
	std::vector<BoidsPolygonEx*> polygons; //其实可以按联通区域分开存分开处理。到时候再优化
	std::vector<Border*> borders;

	rtree_poly_ex rtree;
	rtree_border rtree_borders;

	void drawBoostRing(polygon::ring_type& ring, cocos2d::Color4F color, cocos2d::DrawNode* node);
	void drawBoostPolygonOn(polygon& poly, cocos2d::DrawNode* node);

	bool parseBordersFromBinary(FILE* f);
	void insertBorders(polygon::ring_type& ring, FILE* serializeTo);

	void mergeAndBuildGraph(bool merge = true);

	void createRTree();

	std::map<std::string, std::vector<Border*>*> additional_borders;

	void findCorners(cocos2d::Point start, cocos2d::Point goal, std::vector<PolygonPortal*>& portals, std::vector<cocos2d::Point>& corners);

	void releaseData();

public:
	float _unit_collide_radius;

	NavMesh() {}
	NavMesh(BoidsPolygon& movable, const std::vector<BoidsPolygon*>& barriers, float unit_collide_radius, FILE* serializeTo = nullptr);
	virtual ~NavMesh();

	bool parseFromBinary(FILE* f);

	void addCollidablePolygon(BoidsPolygon& poly);
	bool removeCollidablePolygon(const std::string& name);
	
	BoidsPolygonEx* findPolygonByPoint(cocos2d::Point pt);
	//BoidsPolygonEx* findPolygonByPointWithinBarrier(cocos2d::Point pt); //为一个在建筑内的点找一个最近的Polygon

	void getNearbyBorders(cocos2d::Point pos, float box_radius, std::vector<Collidable*>& output);
			
	bool hasPath(cocos2d::Point from, cocos2d::Point to);
	std::unique_ptr<Path> findPath(cocos2d::Point from, cocos2d::Point to, int path_validity_frame = INT_MAX);

	void drawSketchOn(cocos2d::DrawNode* node);

	static void runTest();
	static void testMergeAndBuildGraph();
	static void testFindPath();
};

#endif