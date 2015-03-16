#include "AITest.h"
#include "CycledList.h"
#include "BoidsMath.h"
#include "BoidsPolygon.h"
#include "BoidsPolygonEx.h"
#include "NavMesh.h"
#include "cocos2d.h"

void AITest::run()
{
	CycledListTest::run();
	Geometry::runTest();
	BoidsPolygon::runTest();
	BoidsPolygonEx::runTest();
	NavMesh::runTest();

	cocos2d::log("AI related tests finished.");
}