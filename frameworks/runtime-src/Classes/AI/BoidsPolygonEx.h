#ifndef __Boids__BoidsPolygonEx__
#define __Boids__BoidsPolygonEx__

#include "cocos2d.h"
#include <vector>
#include "BoidsPolygon.h"
#include "BoostGeometryDef.h"

class BoidsPolygonEx : public BoidsPolygon //有凸化相关的一系列方法
{
public:
	int _id;
	BoidsPolygonEx();
	BoidsPolygonEx(const std::string& polygon_serialization);
	BoidsPolygonEx(const polygon::ring_type& ring);
	virtual ~BoidsPolygonEx();
	
	void processInnerRings(const std::vector<BoidsPolygonEx*>& inner_rings);
	void convexification(std::vector<BoidsPolygonEx*>& output);
	void convexification2(std::vector<BoidsPolygonEx*>& output); //这个是正在使用中的版本
	void convexification3(std::vector<BoidsPolygonEx*>& output); //这个还有bug

	std::vector<class PolygonPortal> portals;

	void serializeToBinary(FILE* f);
	bool parseFromBinary(FILE* f);

	static void runTest();

protected:
	static int global_id;
	void updateConnection(class ShapeConnection& conn, BoidsPolygonEx* inner_ring, PolygonPointPtr outer_begin, PolygonPointPtr outer_end);
	void applyConnection(const class ShapeConnection& conn, PolygonPointPtr& affected_begin, PolygonPointPtr& affected_end);
	
	BoidsPolygonEx* split(PolygonPointPtr split_start, PolygonPointPtr split_end);

	PolygonPointPtr checkInnerIntersectIfConnect(cocos2d::Point pt1, cocos2d::Point pt2, PolygonPointPtr from, PolygonPointPtr to, float s_constraint, float& corresponding_t);
};

enum ShapeConnectionType
{
	connInnerPointToOuterEdge = 0,
	connOuterPointToInnerEdge = 1,
	connOuterPointToInnerPoint = 2
};

class ShapeConnection
{
public:
	PolygonPointPtr inner_pt, outer_pt;
	ShapeConnectionType connection_type; 
	//为0的话，表示从内部点到外部边(记的是起始点）；
	//为1的话，表示从外部点，到内部边（但记的是结束点）；
	//为2的话，表示外部点，到内部点

	cocos2d::Point intersection_point;
	float distance_sqr;
	BoidsPolygonEx* destination_poly;

	ShapeConnection() : distance_sqr(FLT_MAX)
	{
	}
};

class PolygonBorder
{
public:
	PolygonPointPtr pt0, pta, ptb; //点的顺序是pt0, pta, ptb
	BoidsPolygonEx *belong_to;
	PolygonBorder() {}
	PolygonBorder(PolygonPointPtr _pt0, PolygonPointPtr _pta, PolygonPointPtr _ptb, BoidsPolygonEx *_belong_to) : pt0(_pt0), pta(_pta), ptb(_ptb), belong_to(_belong_to)
	{
	}
};

class PolygonPortal
{
public:
	cocos2d::Point a, b; //边的方向是a -> b
	BoidsPolygonEx *from, *to;

	const cocos2d::Point center() const
	{
		return (a + b) * 0.5f;
	}
};

#endif