#include "BoidsPolygon.h"
#include "../BoidsMath.h"
#include "../TMXParser/TmxPolyline.h"
#include "../TMXParser/TmxPolygon.h"
#include "../Utils.h"

using namespace cocos2d;

segment toBoostSeg(PolygonPointPtr src)
{
	return segment(Utils::toPt(src->d), Utils::toPt(src->next->d));
}

segment toBoostSeg2(PolygonPointPtr src)
{
	return segment(Utils::toPt(src->next->d), Utils::toPt(src->d));
}

void initializeBoostPolygon(BoidsPolygon& old_poly, polygon& new_poly)
{
	old_poly.foreach([&](cocos2d::Point p)
	{
		new_poly.outer().push_back(point(p.x, p.y));
	});
}


void BoidsPolygon::loadFromTmxObject(Tmx::Object* ob, float map_height)
{
	name = ob->GetName();
	float base_x = ob->GetX();
	float base_y = ob->GetY();
	auto polyline = ob->GetPolyline();
	if (polyline)
	{
		loadFromTmxPolylineOrPolygon(base_x, base_y, polyline);
	}
	else
	{
		auto polygon = ob->GetPolygon();
		if (polygon)
		{
			loadFromTmxPolylineOrPolygon(base_x, base_y, polygon);
		}
		else
		{ //rectangle
			float width = ob->GetWidth();
			float height = ob->GetHeight();
			initializeAsRectangle(base_x, base_y, width, height);
		}
	}

	foreachref([=](cocos2d::Point& pt)
	{
		pt.y = map_height - pt.y;
	});

	this->makeSureAnticlockwise();
}

void BoidsPolygon::loadFromValueMap( const cocos2d::ValueMap& data, float map_height ) {
    float minx = FLT_MAX, miny = FLT_MAX, maxx = -FLT_MAX, maxy = -FLT_MAX;
    const cocos2d::ValueVector& points = data.at( "points" ).asValueVector();
    float base_x = data.at( "x" ).asFloat();
    float base_y = data.at( "y" ).asFloat();
    if( map_height == 0 ) {
        map_height = data.at( "map_height" ).asFloat();
    }
    
    for( auto itr = points.begin(); itr != points.end(); ++itr ) {
        const cocos2d::ValueMap& pos_pair = itr->asValueMap();
        float x = base_x + pos_pair.at( "x" ).asFloat();
        float y = base_y - pos_pair.at( "y" ).asFloat();
        if( x < minx ) {
            minx = x;
        }
        if( x > maxx ) {
            maxx = x;
        }
        if( y < miny ) {
            miny = y;
        }
        if( y > maxy ) {
            maxy = y;
        }
        this->addNewVertex( x, y );
    }
    this->center = Point( ( minx + maxx ) / 2.0f, ( miny + maxy ) / 2.0f );
    this->makeSureClockwise();
}

void BoidsPolygon::initializeAsRectangle(float x, float y, float width, float height)
{
	push_back(cocos2d::Point(x, y));
	push_back(cocos2d::Point(x + width, y));
	push_back(cocos2d::Point(x + width, y + height));
	push_back(cocos2d::Point(x, y + height));
}

void BoidsPolygon::addNewVertex(float x, float y)
{
	push_back(cocos2d::Point(x, y));
}

BoidsPolygon::BoidsPolygon(const std::string& polygon_serialization)
{
	std::vector<std::string> split_output;
	Utils::split(polygon_serialization, split_output, ' ');
	for (unsigned int i = 0; i < split_output.size(); ++i)
	{
		if (split_output[i].empty()) continue; //如果有空的说明出现了连续空格，认为是多打了，忽略
		float x, y;
		sscanf(split_output[i].c_str(), "(%f,%f)", &x, &y);
		addNewVertex(x, y);
	}
}

std::string BoidsPolygon::serialize()
{
	std::ostringstream os;
	foreach([&](cocos2d::Point pt)
	{
		os << "(" << pt.x << "," << pt.y << ") ";
	});
	std::string ret = os.str();
	ret.pop_back();
	return ret;
}

bool BoidsPolygon::operator == (const class BoidsPolygon& other)
{
	if (_size != other._size) return false;

	bool equal = true;

	PolygonPointPtr p1 = ap;
	PolygonPointPtr p2 = other.ap;
	for (;;)
	{
		if (!p1->d.fuzzyEquals(p2->d, FUZZY_THRESHOLD))
		{
			return false;
		}
		p1 = p1->next;
		if (p1 == ap) break;
		p2 = p2->next;
	}

	return true;
}

void BoidsPolygon::makeSureAnticlockwise(cocos2d::Point outside_point) //假设是简单多边形，即一个点上面最多两条边
{
	//判断在远处的outside点是否在多边形外：是的话就是逆时针，否则就是顺时针。用向量算一遍面积看是正的还是负的

	float area = 0.0f;
	foreach2([&](cocos2d::Point pt, cocos2d::Point pt2)
	{
		area += cocos2d::Vec2(outside_point, pt).cross(cocos2d::Vec2(outside_point, pt2));
	});

	if (area < 0.0f)
	{
		//log("clockwise polygon detected.");
		reverse();
	}
}

void BoidsPolygon::makeSureClockwise(cocos2d::Point outside_point)
{
    
    float area = 0.0f;
    foreach2([&](cocos2d::Point pt, cocos2d::Point pt2)
    {
        area += cocos2d::Vec2(outside_point, pt).cross(cocos2d::Vec2(outside_point, pt2));
    });
    
    if (area >= 0.0f)
    {
        //log("clockwise polygon detected.");
        reverse();
    }
}

//repulsion_vec为斥力向量，当冲突时，表示沿着向量走那么段路就能不碰撞
//本来可以直接扩展障碍物，然后看点是否在多边形内，但为了获得斥力，还是在原多边形上判了
bool BoidsPolygon::isCollide(cocos2d::Point circle_center, float radius, cocos2d::Vec2* repulsion_vec)
{
	float radius_sqr = radius * radius;
	
	bool collide = false;

	int inner_side_count = 0;
	cocos2d::Vec2 smallest_repulsion_when_inside;
	float smallest_repulsion_length_when_inside = FLT_MAX;

	foreach2_with_break([&](cocos2d::Point pt, cocos2d::Point pt2) -> bool
	{
		//多边形的结点是逆时针顺序，所以求逆时针的垂向量肯定是指向内部的
		auto perpendicular_vec = Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Vec2(pt, pt2));
		perpendicular_vec.normalize();
		auto perpendicular_line = perpendicular_vec * radius;
		float s, t;
		cocos2d::Vec2::isLineIntersect(circle_center, circle_center + perpendicular_line, pt, pt2, &s, &t);
		if (Fuzzy::_equalGreater(s, 0.0f) && Fuzzy::_less(s, 1.0f))
		{
			if (Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f)) //和边相交
			{
				if (repulsion_vec)
				{
					*repulsion_vec = -perpendicular_line * (1 - s);
				}
				collide = true;
				return true;
			}
			else if (Fuzzy::_less(t, 0.0f)) //垂线和边的延长线相交，因此要看一下是否在角落处相交
			{
				float dist_sqr = pt.distanceSquared(circle_center);
				if (Fuzzy::_less(dist_sqr, radius_sqr))
				{
					if (repulsion_vec)
					{
						float dist = sqrt(dist_sqr);
						*repulsion_vec = cocos2d::Vec2(pt, circle_center) / dist * (radius - dist);
					}
					collide = true;
					return true;
				}
			}
		}
		else if (Fuzzy::_less(s, 0.0f)) //如果s比0小的话，说明圆心都在线的另一边了，有整个单位都在障碍物内部的嫌疑，统计一下是否所有边都这样
		{
			inner_side_count++;
			float repulsion_length = radius * (1 - s);
			if (repulsion_length < smallest_repulsion_length_when_inside)
			{
				smallest_repulsion_length_when_inside = repulsion_length;
				smallest_repulsion_when_inside = -perpendicular_vec * repulsion_length;
			}
		}
		return false;
	});

	if (collide)
	{
		return true;
	}

	if (inner_side_count == size())
	{
		if (repulsion_vec)
		{
			*repulsion_vec = smallest_repulsion_when_inside;
		}
		return true;
	}
	return false;
}

void BoidsPolygon::getEnlargedEdge(cocos2d::Point a, cocos2d::Point b, float enlarge_radius, cocos2d::Point& new_a, cocos2d::Point& new_b)
{
	auto vector_towards_outside = -Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Vec2(a, b));
	vector_towards_outside.normalize();
	vector_towards_outside *= enlarge_radius;

	new_a = a + vector_towards_outside;
	new_b = b + vector_towards_outside;
}

const std::string BoidsPolygon::_enlarged_name_suffix = "_e";

BoidsPolygon* BoidsPolygon::getEnlargedPolygon(float enlarge_radius)
{
	auto poly = new BoidsPolygon();
	poly->name = name + BoidsPolygon::_enlarged_name_suffix;

	cocos2d::Point last_edge_a, last_edge_b;
	getEnlargedEdge(tail->d, ap->d, enlarge_radius, last_edge_a, last_edge_b);

	foreach2([&](cocos2d::Point pt, cocos2d::Point pt2)
	{
		cocos2d::Point current_edge_a, current_edge_b;
		getEnlargedEdge(pt, pt2, enlarge_radius, current_edge_a, current_edge_b);

		float s;
		if (cocos2d::Vec2::isLineIntersect(last_edge_a, last_edge_b, current_edge_a, current_edge_b, &s))
		{
			auto intersection_point = last_edge_a + s * (last_edge_b - last_edge_a);

			bool sharp_angled = false;
			if (Fuzzy::_greater(cocos2d::Vec2(last_edge_a, last_edge_b).cross(cocos2d::Vec2(current_edge_a, current_edge_b)), 0.0f))
			{
				//是否是较小角(现在只判断了<=90度的)，是的话加边处理。当然前提得是凸的那种角，凹的角不减边就不错了..
				cocos2d::Vec2 last_edge_perpendicular = Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Vec2(last_edge_a, last_edge_b));
				if (Fuzzy::_equalGreater(last_edge_perpendicular.cross(cocos2d::Vec2(current_edge_a, current_edge_b)), 0.0f))
				{
					sharp_angled = true;
				}
			}
			if (sharp_angled)
			{
				cocos2d::Vec2 sharp_point_extending_vec(pt, intersection_point);
				cocos2d::Point additional_edge_center_point = pt + sharp_point_extending_vec.getNormalized() * enlarge_radius;

				cocos2d::Vec2 additional_edge_dir = Geometry::anticlockwisePerpendicularVecToLine(sharp_point_extending_vec);
				float s;
				if (cocos2d::Vec2::isLineIntersect(additional_edge_center_point, additional_edge_center_point + additional_edge_dir, current_edge_a, current_edge_b, &s))
				{
					cocos2d::Point additional_edge_b = additional_edge_center_point + s * additional_edge_dir;
					cocos2d::Point additional_edge_a = additional_edge_center_point - s * additional_edge_dir;
					poly->push_back(additional_edge_a);
					poly->push_back(additional_edge_b); //新加了一条边
				}
				else
				{
					log("additional edge unexpectedly parallel with current edge");
					assert(false);
				}
			}
			else
			{
				//之前的线段和现在新产生线段的交点，作为新多边形的一部分
				poly->push_back(intersection_point);
			}
		}
		else
		{
			log("current edge parallel with last edge, the point on the line will be ignored");
		}

		last_edge_a = current_edge_a;
		last_edge_b = current_edge_b; //last_edge主要是用来判断方向的，具体点的位置倒其实无所谓
	});

	poly->cutConvolution();
	return poly;
}

void BoidsPolygon::cutConvolution()
{
	//检查边相交的情况
	foreachelement2([=](PolygonPointPtr _pt, PolygonPointPtr _pt2)
	{
		cocos2d::Point pt = _pt->d, pt2 = _pt2->d;
		cocos2d::Point intersection_point = PT_INVALID;
		PolygonPointPtr next_pt_after_cut;

		for (PolygonPointPtr to_check_pt1 = _pt2->next, to_check_pt2 = to_check_pt1->next; to_check_pt2 != _pt; to_check_pt1 = to_check_pt1->next, to_check_pt2 = to_check_pt2->next)
		{
			//这里跑了一整个循环，有待优化。其实不严谨点的话遇到第一个相交的就退出是不是也可以

			float s, t;
			if (cocos2d::Vec2::isLineIntersect(pt, pt2, to_check_pt1->d, to_check_pt2->d, &s, &t))
			{
				if (Fuzzy::_equalGreater(s, 0.0f) && Fuzzy::_equalLess(s, 1.0f) && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f))
				{
					intersection_point = pt + s * (pt2 - pt);
					next_pt_after_cut = to_check_pt2; //会不会剪错段，即把凹多边形的大部分给减掉了，剩下的小部分反过来变成了凸多边形。。遇到问题再改- - 可以考虑改成走外轮廓的那种
				}
			}
		}

		if (intersection_point != PT_INVALID)
		{
			//把(_pt, next_pt_after_cut)删掉
			erase(_pt, next_pt_after_cut);
			//然后在之间插入intersection_point
			insert(intersection_point, _pt);
		}
	});
}

bool BoidsPolygon::isConvex()
{
	bool convex = true;
	foreachelement2_with_break([&](PolygonPointPtr ptr1, PolygonPointPtr ptr2)
	{
		cocos2d::Point& pt1 = ptr1->d;
		cocos2d::Point& pt2 = ptr2->d;
		cocos2d::Point& pt3 = ptr2->next->d;
		if (Fuzzy::_less(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(pt2, pt3)), 0.0f)) //反射角
		{
			convex = false;
			return true;
		}
		return false;
	});
	return convex;
}

box BoidsPolygon::getBoundingBox()
{
	float minx = FLT_MAX, miny = FLT_MAX, maxx = -FLT_MAX, maxy = -FLT_MAX;
	foreach([&](cocos2d::Point pt)
	{
		if (pt.x < minx)
		{
			minx = pt.x;
		}
		if (pt.x > maxx)
		{
			maxx = pt.x;
		}
		if (pt.y < miny)
		{
			miny = pt.y;
		}
		if (pt.y > maxy)
		{
			maxy = pt.y;
		}
	});
	return box(point(minx, miny), point(maxx, maxy));
}

bool BoidsPolygon::coversPoint(cocos2d::Point pt)
{
	for (PolygonPointPtr ptr = ap;;)
	{
		cocos2d::Point& pt1 = ptr->d;
		cocos2d::Point& pt2 = ptr->next->d;
		if (Fuzzy::_less(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(pt1, pt)), 0.0f))
		{
			return false;
		}
		ptr = ptr->next;
		if (ptr == ap) break;
	}
	return true;
}

void BoidsPolygon::testIsCollide()
{
	{
		BoidsPolygon poly;
		poly.push_back(cocos2d::Point::ZERO);
		poly.push_back(cocos2d::Point(0.0f, -1.5f));
		poly.push_back(cocos2d::Point(2.0f, -1.5f));
		poly.push_back(cocos2d::Point(2.0f, 0.0f));

		{ //在角上的情况
			cocos2d::Point circle_center(-0.2f, 0.4f);
			float radius = 1.0f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //穿过多边形的情况
			cocos2d::Point circle_center(1.0f, -0.75f);
			float radius = 1.0f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //在多边形内的情况
			cocos2d::Point circle_center(1.0f, -0.6f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //在多边形左侧的情况
			cocos2d::Point circle_center(-0.49f, -0.75f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //在多边形上侧的情况
			cocos2d::Point circle_center(1.8f, 0.45f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}
	}

	{
		BoidsPolygon poly;
		poly.push_back(cocos2d::Point(0.0, 2.0f));
		poly.push_back(cocos2d::Point(-1.0f, 0.0f));
		poly.push_back(cocos2d::Point(0.0f, -2.0f));
		poly.push_back(cocos2d::Point(1.0f, 0.0f));

		{ //在多边形右上侧的情况
			cocos2d::Point circle_center(1.0f, 1.0f);
			float radius = 1.0f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //在多边形左下侧的情况
			cocos2d::Point circle_center(-1.0f, -1.0f);
			float radius = 1.0f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}
	}
	
	{ //测试凹多边形
		BoidsPolygon poly;
		poly.push_back(cocos2d::Point::ZERO);
		poly.push_back(cocos2d::Point(2.0f, 0.0f));
		poly.push_back(cocos2d::Point(2.0f, 1.0f));
		poly.push_back(cocos2d::Point::ONE);
		poly.push_back(cocos2d::Point(1.0f, 2.0f));
		poly.push_back(cocos2d::Point(0.0, 2.0f));
		
		{ //在多边形右上侧的情况（刚好放到凹口上）
			cocos2d::Point circle_center(1.5f, 1.5f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(!collide);
		}

		{ //在多边形右上侧的情况（放到凹口里）
			cocos2d::Point circle_center(1.0f, 1.0f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			circle_center += repulsion;
			if (poly.isCollide(circle_center, radius, &repulsion)) //凹多边形如果和两个面相交，要repulsion两次才能不碰撞。其它情况倒是好的。。
			{
				collide = poly.isCollide(circle_center + repulsion, radius, &repulsion);
				assert(!collide);
			}
		}
		
		{ //在多边形右上侧的情况（凹口里，偏上）
			cocos2d::Point circle_center(1.2f, 1.5f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //在多边形右上侧的情况（凹口里，偏下）
			cocos2d::Point circle_center(1.5f, 1.2f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}

		{ //在多边形右侧
			cocos2d::Point circle_center(2.45f, 0.5f);
			float radius = 0.5f;
			cocos2d::Vec2 repulsion;
			bool collide = poly.isCollide(circle_center, radius, &repulsion);
			assert(collide);
			assert(!poly.isCollide(circle_center + repulsion, radius));
		}
	}	
}

void BoidsPolygon::testEnlarge()
{
	{
		BoidsPolygon poly;
		poly.initializeAsRectangle(0.0f, 0.0f, 1.0f, 2.0f);
		auto new_poly = poly.getEnlargedPolygon(1.0f);		
		delete new_poly;
	}
}

void BoidsPolygon::runTest()
{
	testIsCollide();
	testEnlarge();
}

void BoidsPolygon::drawSketchOn(cocos2d::DrawNode* node, cocos2d::Color4F c)
{
	foreach2([=](cocos2d::Point pt, cocos2d::Point pt2)
	{
		Utils::drawVec(pt, pt2, node, c, true);
	});

	for (unsigned int i = 0; i < debug_lines.size(); ++i)
	{
		Utils::drawVec(debug_lines[i].src, debug_lines[i].dst, node, Color4F::GREEN);
	}
}

std::vector<Border> BoidsPolygon::getNearbyBorders( const cocos2d::Point& center, float radius ) {
    std::vector<Border> ret;
    
    foreachelement2( [&](PolygonPointPtr _pt, PolygonPointPtr _pt2) {
        Point pt1 = _pt->d;
        Point pt2 = _pt2->d;
        Point pt3 = _pt2->next->d;
        
        if( Geometry::distanceToLine( center, pt1, pt2 ) <= radius ) {
            ret.push_back( Border( pt1, pt2, pt3 ) );
        }
    } );
    
    return ret;
}