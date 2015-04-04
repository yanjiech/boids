#ifndef __Boids__BoidsPolygon__
#define __Boids__BoidsPolygon__

#include "cocos2d.h"
#include <algorithm>
#include <TmxPoint.h>
#include <TmxObject.h>
#include "BoidsMath.h"
#include "CycledList.h"
#include "BoostGeometryDef.h"
#include "Utils.h"
#include "Border.h"

typedef CycledListElement<cocos2d::Point>* PolygonPointPtr;

segment toBoostSeg(PolygonPointPtr src);
segment toBoostSeg2(PolygonPointPtr src);

class BoidsPolygon : public CycledList<cocos2d::Point> //有除了凸化以外的方法
{
protected:
	static void testIsCollide();
	static void testEnlarge();

	void cutConvolution();

	template <typename P>
	void loadFromTmxPolylineOrPolygon(float base_x, float base_y, P polyline)
	{
		assert(polyline->GetNumPoints() >= 2);

		for (int i = 0; i < polyline->GetNumPoints(); ++i)
		{
			const Tmx::Point& pt = polyline->GetPoint(i);
			cocos2d::Point ccpt(base_x + pt.x, base_y + pt.y);
			push_back(ccpt);
		}
	}

	void getEnlargedEdge(cocos2d::Point a, cocos2d::Point b, float enlarge_radius, cocos2d::Point& new_a, cocos2d::Point& new_b);

public:
	std::string name;
	static const std::string _enlarged_name_suffix;

	BoidsPolygon() {}
	void addNewVertex(float x, float y);
	BoidsPolygon(const std::string& polygon_serialization);
	std::string serialize();
	bool operator == (const class BoidsPolygon& other);

	void loadFromTmxObject(Tmx::Object* ob, float map_height);
    
    void loadFromValueMap( const cocos2d::ValueMap& data, float map_height = 0 );

	void initializeAsRectangle(float x, float y, float width, float height);

	void makeSureAnticlockwise(cocos2d::Point outside_point = cocos2d::Point(-1000.0f, -1000.0f));
    void makeSureClockwise(cocos2d::Point outside_point = cocos2d::Point(-1000.0f, -1000.0f));

	bool isCollide(cocos2d::Point circle_center, float radius, cocos2d::Vec2* repulsion_vec = nullptr);

	bool isConvex(); //O(n)时间判断; 得保证多边形是逆时针顺序

	box getBoundingBox();

	bool coversPoint(cocos2d::Point pt);

	BoidsPolygon* getEnlargedPolygon(float enlarge_radius);

	std::vector<Line> debug_lines;
    
    std::vector<Border> getNearbyBorders( const cocos2d::Point& center, float radius );

	void drawSketchOn(cocos2d::DrawNode* node, cocos2d::Color4F c = cocos2d::Color4F::BLUE);

	static void runTest();
};

void initializeBoostPolygon(BoidsPolygon& old_poly, polygon& new_poly);

#endif