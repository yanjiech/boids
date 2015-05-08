#include "AI.h"
#include "AIUnit.h"
#include "Terrain.h"
#include <algorithm>
#include "BoidsMath.h"
#include "CycledList.h"
#include "BoidsPolygon.h"
#include "BoidsPolygonEx.h"
#include "Utils.h"
#include "Trace.h"
#include "CmdLineDebugger.h"
#include "AITest.h"
#include "Counter.h"

AI* AI::_instance = nullptr;

AI::AI() : _step_draw_node(nullptr)
{
	reset();
}

void AI::initialize(cocos2d::ValueMap constants)
{
	AITest::run();	

	if (constants.count("BUILDING_PRIORITY"))
	{
		_BUILDING_PRIORITY = constants["BUILDING_PRIORITY"].asInt();
	}
	else
	{
		_BUILDING_PRIORITY = 10000;
	}

	if (constants.count("MINIMAL_SPLIT_DISTANCE"))
	{
		_MINIMAL_SPLIT_DISTANCE = constants["MINIMAL_SPLIT_DISTANCE"].asFloat();
	}
	else
	{
		_MINIMAL_SPLIT_DISTANCE = 80;
	}

	if (constants.count("STOP_CATCHUP_DISTANCE"))
	{
		_STOP_CATCHUP_DISTANCE = constants["STOP_CATCHUP_DISTANCE"].asFloat();
	}
	else
	{
		_STOP_CATCHUP_DISTANCE = 80;
	}

	if (constants.count("MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW"))
	{
		_MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW = constants["MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW"].asFloat();
	}
	else
	{
		_MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW = 50;
	}

	if (constants.count("STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW"))
	{
		_STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW = constants["STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW"].asFloat();
	}
	else
	{
		_STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW = 80;
	}

	if (constants.count("PAINT_PATH"))
	{
		_PAINT_PATH = constants["PAINT_PATH"].asBool();
	}
	else
	{
		_PAINT_PATH = false;
	}

	if (constants.count("PAINT_MESH"))
	{
		_PAINT_MESH = constants["PAINT_MESH"].asInt();
	}
	else
	{
		_PAINT_MESH = 0;
	}

	if (constants.count("PAINT_STEP"))
	{
		_PAINT_STEP = constants["PAINT_STEP"].asBool();
	}
	else
	{
		_PAINT_STEP = false;
	}

	if (constants.count("TICK"))
	{
		_TICK = constants["TICK"].asInt();
	}
	else
	{
		_TICK = 0;
	}

	if (constants.count("NOT_LOAD_NAV"))
	{
		_NOT_LOAD_NAV = constants["NOT_LOAD_NAV"].asBool();
	}
	else
	{
		_NOT_LOAD_NAV = false;
	}

	if (constants.count("ENABLE_PUSH"))
	{
		_ENABLE_PUSH = constants["ENABLE_PUSH"].asBool();
	}
	else
	{
		_ENABLE_PUSH = false;
	}
}

void AI::updateFrame()
{
	_current_frame++;

	if (_TICK && _current_frame % _TICK == 0)
	{
		Utils::logAI("tick");
	}
}

bool AI::parseMap(std::string tmx_file_content, std::vector<float> all_collide_radius)
{
	log("====parseMap====");
	std::set<float> all_radius;
	for (unsigned int i = 0; i < all_collide_radius.size(); ++i)
	{
		all_radius.insert(all_collide_radius[i]);
	}
	return Terrain::getInstance()->parseMap(tmx_file_content, all_radius);
}

void AI::releaseCurrentMap()  //本方法可能被重复调用
{
	//释放地图
	Terrain::getInstance()->release();
}

//bool AI::isPositionOK(cocos2d::Point pos, float radius)
//{
//	auto mesh = Terrain::getInstance()->getMeshByUnitRadius(radius);
//	if (mesh->findPolygonByPoint(pos))
//	{
//		std::vector<Collidable*> collidables;
//		mesh->getNearbyBorders(pos, radius, collidables);
//
//		for (auto& id_u : _units)
//		{
//			collidables.push_back(id_u.second);
//		}
//		for (auto u : extra_collidables)
//		{
//			collidables.push_back(u);
//		}
//
//		for (auto c : collidables)
//		{
//			if (c->willCollide(pos, radius))
//			{
//				return false;
//			}
//		}
//		return true;
//	}
//	return false;
//}
//
//cocos2d::ValueVector AI::getAvailablePosition(std::vector<float> radius, cocos2d::Rect region, int group_count)
//{
//	assert(region.size.width > 0 && "region_width is zero");
//	assert(region.size.height > 0 && "region_height is zero");
//
//	cocos2d::Rect original_region = region;
//
//#define RETRY_TIMES_BEFORE_ENLARGE (10)
//#define MAX_ENLARGE_TIMES (30) //扩展30次还没位置的话，九成是地图有问题啊。。
//
//	unsigned extra_collidables_count_bak = extra_collidables.size();
//
//	int enlarge_time = 0;
//
//	for (int c = 0; c != radius.size(); ++c)
//	{
//		bool ok = false;
//		for (;;)
//		{
//			for (int t = 0; t != RETRY_TIMES_BEFORE_ENLARGE; ++t)
//			{
//				float y_pos = region.getMinY() + Utils::randomNumber((int)region.size.height) - 1;
//				float x_pos = region.getMinX() + Utils::randomNumber((int)region.size.width) - 1;
//				cocos2d::Point pt(x_pos, y_pos);
//				char buf[400];
//				sprintf(buf, "getAvailablePos, radius: %.1f; region: %.2f, %.2f, %.2f, %.2f; result: %.2f, %.2f",
//					radius[c], region.getMinX(), region.getMinY(), region.size.width, region.size.height, pt.x, pt.y);
//				Utils::logr(buf, true);
//				if (isPositionOK(pt, radius[c]))
//				{
//					ok = true;
//					AIUnit* u = new AIUnit();
//					u->set_position(pt.x, pt.y);
//					u->set_collide_radius(radius[c]);
//					extra_collidables.push_back(u);
//					break;
//				}
//			}
//			if (ok) break;
//
//			enlarge_time++;
//			if (enlarge_time > MAX_ENLARGE_TIMES)
//			{
//				char buf[500];
//				sprintf(buf, "getAvailablePosition failed. rect: x:%.2f, y: %.2f, width: %.2f, height: %.2f",
//					original_region.getMinX(), original_region.getMinY(), original_region.size.width, original_region.size.height);
//				cocos2d::log("%s", buf);
//				assert(false);
//			}
//
//			float step = radius[c] * 2.0f;
//			region.setRect(region.getMinX() - step, region.getMinY() - step, region.size.width + step * 2.0f, region.size.height + step * 2.0f);
//		}
//	}
//
//	cocos2d::ValueVector positions;
//	for (unsigned i = extra_collidables_count_bak; i != extra_collidables.size(); ++i)
//	{
//		auto u = extra_collidables[i];
//		cocos2d::ValueMap pos;
//		pos["x"] = u->get_position().x;
//		pos["y"] = u->get_position().y;
//		positions.push_back(cocos2d::Value(pos));
//
//		delete u;
//	}
//	extra_collidables.resize(extra_collidables_count_bak);
//
//	return positions;
//}

void AI::addCollidablePolygon(std::string name, std::vector<float> points_x, std::vector<float> points_y)
{
	Terrain::getInstance()->addCollidablePolygon(name, points_x, points_y);
}

void AI::addCollidablePolygon(std::string name)
{
	Terrain::getInstance()->addCollidablePolygon(name);
}

bool AI::removeCollidablePolygon(std::string name)
{
	return Terrain::getInstance()->removeCollidablePolygon(name);
}

void AI::paintDebugLayer()
{
	Terrain::getInstance()->paintDebugLayer();

	auto layer = Utils::getCurrentMapLayer();
	if (layer)
	{
		if (_PAINT_STEP)
		{
			_step_draw_node = DrawNode::create();
			layer->addChild(_step_draw_node, 1500);
		}
	}
}

class UnitDistance
{
public:
	AIUnit *a, *b;
	float distanceSqr;

	UnitDistance(AIUnit* _a, AIUnit* _b) : a(_a), b(_b), distanceSqr(a->get_position().getDistanceSq(b->get_position()))
	{
	}

	bool operator < (const UnitDistance& dist2)
	{
		return distanceSqr < dist2.distanceSqr;
	}
};

AIUnit* getFather(std::map<AIUnit*, AIUnit*>& father_map, AIUnit* u)
{
	auto it = father_map.find(u);
	if (it == father_map.end())
	{
		return u;
	}
	else
	{
		AIUnit* ancestor = getFather(father_map, it->second);
		if (ancestor != it->second)
		{
			it->second = ancestor;
		}
		return ancestor;
	}
}

bool AI::splitIntoTwoGroups(std::string player_camp, float dir_x, float dir_y)
{
//	_lead_group.clear();
//	_fell_behind_group.clear();
//
//	std::vector<AIUnit*> player_units;
//	for (auto& id_u : _units)
//	{
//		if (id_u.second->get_unit_camp() == player_camp)
//		{
//			player_units.push_back(id_u.second);
//		}
//	}
//	if (player_units.size() < 2) return false;
//
//	_in_narrow = false;
//	for (const auto& u : player_units)
//	{
//		if (Terrain::getInstance()->isInNarrowPlace(u->get_position()))
//		{
//			Utils::logAI("%d in narrow", u->get_unit_id());
//			_in_narrow = true;
//			break;
//		}
//	}
//
//	cocos2d::Vec2 joystick_dir(dir_x, dir_y);
//	cocos2d::Vec2 narrow_vec = cocos2d::Vec2(_narrow_dir.first, _narrow_dir.second);
//	cocos2d::Point narrow_midpoint = _narrow_dir.first.getMidpoint(_narrow_dir.second);
//	cocos2d::Vec2 vec_across_narrow;
//	bool walk_along_narrow = false;
//	if (_in_narrow)
//	{
//		if (!_narrow_dir.first.equals(cocos2d::Vec2::ZERO)) //先确保narrow_dir有值
//		{
//			if (Geometry::deviateLessThanAngle(joystick_dir, narrow_vec, 30)) //是朝着narrow_dir方向的
//			{
//				walk_along_narrow = true;
//				vec_across_narrow = Geometry::anticlockwisePerpendicularVecToLine(narrow_vec);
//				_catch_up_position = _narrow_dir.second;
//				Utils::logAI("walk_along_narrow: %.3f, %.3f", _catch_up_position.x, _catch_up_position.y);
//			}
//			else if (Geometry::deviateLessThanAngle(joystick_dir, -narrow_vec, 30)) //朝着narrow_dir的负方向，即从narrow离开
//			{
//				walk_along_narrow = true;
//				vec_across_narrow = Geometry::clockwisePerpendicularVecToLine(narrow_vec);
//				_catch_up_position = _narrow_dir.first;
//				Utils::logAI("walk_along_narrow: %.3f, %.3f", _catch_up_position.x, _catch_up_position.y);
//			}
//		}
//		if (!walk_along_narrow)
//		{
//			Utils::logAI("in narrow but not walk along narrow");
//		}
//	}
//
//	if (walk_along_narrow)
//	{
//		//在vec_across_narrow左侧的得朝着narrow的那头走
//		for (auto u : player_units)
//		{
//			if (vec_across_narrow.cross(cocos2d::Vec2(narrow_midpoint, u->get_position())) > 0.0f)
//			{
//				_fell_behind_group.push_back(u->get_unit_id());
//			}
//			else
//			{
//				_lead_group.push_back(u->get_unit_id());
//			}
//		}
//		if (_fell_behind_group.empty()) //如果都已经过线了，那么就不算walk_along_narrow，得按正常逻辑。其实还可能有部分没过线，部分可能离很远的情况(或者已经过了，或者还没过)
//		{
//			walk_along_narrow = false;
//		}
//		else
//		{
//			return true;
//		}
//	}
//	
//	//进到后面的或者没有遇到窄路，或者遇到窄路但都已经过线了
//	//用kruscal少连一条边分成两堆。感觉是对的
//
//	std::map<AIUnit*, AIUnit*> father;
//
//	std::vector<UnitDistance> distances;
//	for (unsigned i = 0; i != player_units.size(); ++i)
//	{
//		for (unsigned j = i + 1; j != player_units.size(); ++j)
//		{
//			distances.push_back(UnitDistance(player_units[i], player_units[j]));
//		}
//	}
//
//	std::sort(distances.begin(), distances.end(), [](const UnitDistance& a, const UnitDistance& b) {
//		return a.distanceSqr < b.distanceSqr;
//	});
//
//	AIUnit *group_a_ancestor = nullptr;
//
//	int connected_edges = 0;
//	for (const auto& dist : distances)
//	{
//		AIUnit* ancestor_a = getFather(father, dist.a);
//		AIUnit* ancestor_b = getFather(father, dist.b);
//		if (ancestor_a != ancestor_b)
//		{
//			connected_edges++;
//			if (connected_edges + 1 == player_units.size()) //次数到了，算法完成
//			{
//				if (_in_narrow)
//				{
//					if (dist.distanceSqr < _MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW * _MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW) //如果最后连的这条边也比_MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW要小，那就分割失败。
//					{
//						return false;
//					}
//				}
//				else
//				{
//					if (dist.distanceSqr < _MINIMAL_SPLIT_DISTANCE * _MINIMAL_SPLIT_DISTANCE) //如果最后连的这条边也比_MINIMAL_SPLIT_DISTANCE要小，那就分割失败。
//					{
//						return false;
//					}
//				}
//				group_a_ancestor = ancestor_a;
//				break;
//			}
//
//			father[ancestor_b] = ancestor_a;
//		}
//	}
//	assert(group_a_ancestor); 
//
//	std::vector<int> _group_a, _group_b;
//	cocos2d::Point group_a_center, group_b_center;
//	for (auto u : player_units)
//	{
//		AIUnit* ancestor = getFather(father, u);
//		if (ancestor == group_a_ancestor)
//		{
//			_group_a.push_back(u->get_unit_id());
//			group_a_center += u->get_position();
//		}
//		else
//		{
//			_group_b.push_back(u->get_unit_id());
//			group_b_center += u->get_position();
//		}
//	}
//	group_a_center *= 1.0f / _group_a.size();
//	group_b_center *= 1.0f / _group_b.size();
//
//	cocos2d::Vec2 cutting_line = Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Point(group_a_center, group_b_center));
//	if (cutting_line.cross(joystick_dir) >= 0.0f)
//	{
//		_lead_group = _group_a;
//		_fell_behind_group = _group_b;
//		_catch_up_position = group_a_center;
//	}
//	else
//	{
//		_lead_group = _group_b;
//		_fell_behind_group = _group_a;
//		_catch_up_position = group_b_center;
//	}
//
	return true;
}

//=========性能测试===========
#define __TESTCASE (300000)

void AI::test_c_whole(int test_id)
{
	if (test_id == 1)
	{
		//Trace t;
		cocos2d::Point pt(Utils::randomNumber2(1000), Utils::randomNumber2(1000));
		float sum = 0.0f;
		for (unsigned t = 0; t != __TESTCASE; ++t)
		{
			sum += pt.distanceSquared(cocos2d::Point(t, t));
		}
		//t.trace("test1_c_whole");
		//Utils::logAI("test1_c_whole result: %f", sum);
	}
	else if (test_id == 2)
	{
		//Trace t;
		int sum = 0;
		for (unsigned t = 0; t != __TESTCASE; ++t)
		{
			sum += t;
		}
		//t.trace("test2_c_whole");
		//Utils::logAI("test2_c_whole result: %d", sum);
	}
}

cocos2d::Point AI::test1_pos(int t)
{
	return cocos2d::Point(t, t);
}

int AI::test2_int(int t)
{
	return t;
}