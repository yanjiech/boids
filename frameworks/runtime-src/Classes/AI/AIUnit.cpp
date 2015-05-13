#include "AIUnit.h"
#include "Terrain.h"
#include "Trace.h"
#include "Utils.h"
#include <memory>
#include "GrassMonitor.h"
#include <sstream>
#include "Counter.h"
#include "Border.h"

#define _LOG_POS_

inline float sqr(float x)
{
	return x * x;
}

void AIUnit::initialize(int unit_id)
{
	_unit_id = unit_id;
	AI::getInstance()->_units.emplace(unit_id, this);
}

void AIUnit::deleteUnit()
{
	unsigned erase_count = AI::getInstance()->_units.erase(_unit_id);
	Utils::logAI("units erase count: %d id(%d)", erase_count, _unit_id);
}

AIUnit::AIUnit() : _during_catchup(0), _hesitate_frame(0)
{
	resetLastWalk();
}

AIUnit::~AIUnit()
{
	//Utils::logAI("delete unit: %d", _unit_id);
}

void AIUnit::paintPath()
{
	if (path)
	{
		Utils::logAI("paint path: %d", _unit_id);
		auto node = Utils::getDrawNode();
		if (node)
		{
			path->paint(_position, node, cocos2d::Color4F::ORANGE);
		}
	}
	else
	{
		Utils::logAI("no path to paint: %d", _unit_id);
	}
}

bool AIUnit::hasPath()
{
	return path.operator bool();
}

void AIUnit::resetPath()
{
	path.reset();
}

bool AIUnit::isPathOutdated()
{
	return path->isOutdated();
}

bool AIUnit::isPathEnd()
{
	return path->isPathEnd();
}

void AIUnit::raw_findPath(cocos2d::Point dest, int path_validity_frame)
{
	path = Terrain::getInstance()->getMeshByUnitRadius(_collide_radius)->findPath(_position, dest, path_validity_frame);
}

void AIUnit::findPath(float dest_x, float dest_y, int path_validity_frame)
{
	cocos2d::Point dest(dest_x, dest_y);
	raw_findPath(dest, path_validity_frame);
}

void AIUnit::findPathWithDestRadius(float dest_x, float dest_y, float dest_radius, int path_validity_frame)
{
	cocos2d::Point dest(dest_x, dest_y);
	raw_findPath(dest, path_validity_frame);
	path->customized_is_reach_destination = [=](cocos2d::Point pos) {
		return pos.getDistanceSq(dest) <= sqr(dest_radius);
	};
}

void AIUnit::findPathForTarget(AIUnit* target, int path_validity_frame)
{
	raw_findPath(target->get_position(), path_validity_frame);
}

void AIUnit::findPathForTargetWithRadius(AIUnit* target, float dest_radius, int path_validity_frame)
{
	raw_findPath(target->get_position(), path_validity_frame);
	path->customized_is_reach_destination = [=](cocos2d::Point pos) {
		return pos.getDistanceSq(target->get_position()) <= sqr(dest_radius);
	};
}

std::string AIUnit::serializeUnitsVector(const std::vector<AIUnit*>& vec)
{
	std::ostringstream os;
	os << "[";
	for (unsigned i = 0; i != vec.size(); ++i)
	{
		if (i) os << ",";
		os << vec[i]->_unit_id;
	}
	os << "]";

	return os.str();
}

void AIUnit::catchup(float dest_x, float dest_y)
{
	_during_catchup = AI::getInstance()->get_current_frame();
	cocos2d::Point dest(dest_x, dest_y);
	raw_findPath(dest, INT_MAX); //如果遥杆不松的话其实是每帧在寻路，但万一这时候松遥杆的话，掉队的人还是会赶到附近
	//Utils::logAI("unit %d: start catchup. my_pos: %.3f, %.3f. path: %s", _unit_id, _position.x, _position.y, path->serialize().c_str());

	float stop_catchup_distance_sqr; //先根据现状把值取好，再拷到lambda里面
	if (AI::getInstance()->_in_narrow)
	{
		stop_catchup_distance_sqr = sqr(AI::getInstance()->_STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW);
	}
	else
	{
		stop_catchup_distance_sqr = sqr(AI::getInstance()->_STOP_CATCHUP_DISTANCE);
	}

	path->customized_is_reach_destination = [=](cocos2d::Point pos) {
		if (pos.getDistanceSq(cocos2d::Point(dest_x, dest_y)) <= stop_catchup_distance_sqr)
		{
			//Utils::logAI("%d reach catchup destination: %.3f, %.3f", _unit_id, dest_x, dest_y);
			return true;
		}
		else
		{
			return false;
		}
	};
	int current_frame = AI::getInstance()->_current_frame;
	path->path_released_callback = [=]() {
		if (this->_during_catchup == current_frame) //this是拷的，那么during_catchup就是最新的值。帧号如果不同，那说明已经又发起新的catchup了，不用将during置空
		{
			//Utils::logAI("unit %d: end of catchup.", this->_unit_id);
			this->_during_catchup = 0;
		}
	};
}

bool AIUnit::inRangeAndNotBlocked(AIUnit* other, float range)
{
	if (_position.distanceSquared(other->_position) < sqr(range + other->_collide_radius))
	{
		if (!Terrain::getInstance()->isBlocked(_position, other->_position))
		{
			return true;
		}
	}
	return false;
}

bool AIUnit::buildingInRangeAndNotBlocked(float building_x, float building_y, float building_radius, float range)
{
	cocos2d::Point building_pos(building_x, building_y);
	cocos2d::Vec2 vec_towards_building(_position, building_pos);
	float len_sq = vec_towards_building.getLengthSq();
	if (len_sq < sqr(range + building_radius))
	{
		float len = sqrt(len_sq);
		if (!Terrain::getInstance()->isBlocked(_position, _position + vec_towards_building * ((len - building_radius) / len)))
		{
			return true;
		}
	}
	return false;
}

bool AIUnit::inDirectView(AIUnit* other) 
{
	return inRangeAndNotBlocked(other, _guard_radius);
}

bool AIUnit::canAttack(AIUnit* other)
{
	return inRangeAndNotBlocked(other, _attack_radius);
}

bool AIUnit::buildingInDirectView(float building_x, float building_y, float building_radius)
{
	return buildingInRangeAndNotBlocked(building_x, building_y, building_radius, _guard_radius);
}

bool AIUnit::canAttackBuilding(float building_x, float building_y, float building_radius)
{
	return buildingInRangeAndNotBlocked(building_x, building_y, building_radius, _attack_radius);
}

void AIUnit::resetLastWalk()
{
	_last_walk_faceto = -1;
	_last_last_faceto = -1;
}

bool AIUnit::isOscillate(int faceto)
{
	if (faceto == _last_last_faceto)
	{
		if (AI::getInstance()->_current_frame > _last_faceto_frame + _hesitate_frame) //犹豫期已经过了，不算甩头
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool AIUnit::walkFrameOnPath() //调了这个方法的话外面肯定保证有路可走的
{
	return walkPos(path->walkFrame(_position, _max_walk_length));
//	cocos2d::Vec2 unit_separation_force;
//
//#define _UNIT_SEPARATION_FACTOR (0.5f)
//#define _UNIT_SEPARATION_NUMERATOR (100.0f)
//
//	int affected_by = 0, affected_by_b = 0;
//
//	for (auto u : AI::getInstance()->_units)
//	{
//		if (u == this) continue;
//
//		//要把可攻击的给判掉，不用考虑可攻击者的separtion_force，因为我就是要冲过去打的 TODO
//
//		cocos2d::Vec2 dist = u->_position - _position;
//		if (dist.lengthSquared() < sqr(_view_radius))
//		{
//			unit_separation_force += dist * (_collide_radius + _UNIT_SEPARATION_NUMERATOR) / dist.lengthSquared(); //其实是normalized_dist * ((r + numerator) / dist_len)
//			affected_by++;
//		}
//	}
//
//	//建筑的separtion_force
//	cocos2d::Vec2 building_separation_force;
//#define _BUILDING_SEPARATION_FACTOR (0.5f)
//#define _BUIDING_SEPARATION_NUMERATOR (100.0f)
//#define _BUIDING_SEPARATION_DENOMINATOR (10.0f)
//
//	auto mesh = Terrain::getInstance()->getMeshByUnitRadius(_collide_radius);
//	std::vector<Collidable*> collidables;
//	mesh->getNearbyBorders(_position, _view_radius, collidables);
//	for (auto c : collidables)
//	{
//		Border* b = (Border*)c;
//		cocos2d::Vec2 dist = -Geometry::distanceVecToLine(_position, b->src, b->dst);
//		if (dist.lengthSquared() < sqr(_view_radius))
//		{
//			float dist_len = dist.getLength();
//			building_separation_force += dist * (_UNIT_SEPARATION_NUMERATOR / dist_len / (_BUIDING_SEPARATION_DENOMINATOR + dist_len)); //其实是normalized_dist * (numerator / (denumerator + dist_len)
//			affected_by_b++;
//		}
//	}
//	if (affected_by_b)
//	{
//		building_separation_force *= 1.0f / affected_by_b;
//	}
//
//	cocos2d::Vec2 dir_by_path(_position, path->walkFrame(_position, _max_walk_length));
//	cocos2d::Vec2 separation_force = unit_separation_force * _UNIT_SEPARATION_FACTOR + building_separation_force * _BUILDING_SEPARATION_FACTOR;
//	//Utils::logAI("[%d] sp_force_by_units: %.2f, %.2f; _by_buildings: %.2f, %.2f", _unit_id, unit_separation_force.x, unit_separation_force.y, building_separation_force.x, building_separation_force.y);
//
//	if (separation_force.getLengthSq() > dir_by_path.getLengthSq())
//	{
//		separation_force *= sqrt(dir_by_path.getLengthSq() / separation_force.getLengthSq());
//	}
//	if ((dir_by_path + separation_force).fuzzyEquals(cocos2d::Vec2::ZERO, FUZZY_THRESHOLD))
//	{
//		//Utils::logAI("[%d] dir_by_path: %.2f, %.2f; separation: %.2f, %.2f; affected_by: %d; affected_by_b: %d; IGNORE", 
//		//	_unit_id, dir_by_path.x, dir_by_path.y, separation_force.x, separation_force.y, affected_by, affected_by_b);
//		walkPos(_position + dir_by_path);
//	}
//	else
//	{
//		//Utils::logAI("[%d] dir_by_path: %.2f, %.2f; separation: %.2f, %.2f; affected_by: %d; affected_by_b: %d", 
//		//	_unit_id, dir_by_path.x, dir_by_path.y, separation_force.x, separation_force.y, affected_by, affected_by_b);
//		walkPos(_position + (dir_by_path + separation_force).getNormalized() * _max_walk_length);
//	}
}

bool AIUnit::walkStep(float step_x, float step_y)
{
	return walkPos(_position + cocos2d::Vec2(step_x, step_y));
}

bool AIUnit::walkPos(cocos2d::Point pt)
{
	if (AI::getInstance()->_step_draw_node)
	{
		AI::getInstance()->_step_draw_node->drawLine(_position, pt, cocos2d::Color4F(0.0f, 1.0f, 0.1f, 1.0f));
	}

	//ACCUMULATE("before_walkPos");
	bool ret = tryWalk(pt, cocos2d::Vec2(_position, pt));
	if (ret)
	{
		if (_hesitate_frame) //hesitate_frame非0的话就要考虑甩头问题
		{
			cocos2d::Vec2 dir(_position, pt);
			//int faceto = (dir.y > 0) * 2 + (dir.x > 0);
			int faceto = dir.x > 0; //假设都是正面的，于是主要考虑左右甩的问题
			if (isOscillate(faceto))
			{
				restorePush(false);
				_move_type = MoveType::typeStay;
#ifdef _LOG_POS_
				_pos_history.push_back(std::make_pair(AI::getInstance()->_current_frame, _position));
#endif
				//DEBUG
				if (_unit_camp == "player" || _unit_camp == "opponent")
				{
					char buf[400];
					sprintf(buf, "unit %d stay to avoid oscillate.", _unit_id);
					Utils::logr(buf, true);
				}
				return false;

				//现在这样的防甩头机制可能导致增加原地等待（犹豫）的可能；但因为甩头本来就是前面有东西挡着引起的，犹豫一下通常是合情合理的吧。。
			}
			else
			{
				if (faceto != _last_walk_faceto)
				{
					_last_last_faceto = _last_walk_faceto;
					_last_faceto_frame = AI::getInstance()->_current_frame;
					_last_walk_faceto = faceto;
					_last_walk_unit_dir = dir.getNormalized();
				}
			}
		}
		_position = pt;
		_move_type = MoveType::typeInitiative;
#ifdef _LOG_POS_
		_pos_history.push_back(std::make_pair(AI::getInstance()->_current_frame, _position));
#endif
		//DEBUG
		if (_unit_camp == "player" || _unit_camp == "opponent")
		{
			char buf[400];
			sprintf(buf, "unit %d eventually walk to %.4f, %.4f", _unit_id, _position.x, _position.y);
			Utils::logr(buf, true);
		}
	}
	else
	{
		restorePush(false);
		_move_type = MoveType::typeStuck;
		_dir_want_to_go = pt;
#ifdef _LOG_POS_
		_pos_history.push_back(std::make_pair(AI::getInstance()->_current_frame, _position));
#endif
		if (_unit_camp == "player" || _unit_camp == "opponent")
		{
			char buf[400];
			sprintf(buf, "unit %d stuck.", _unit_id);
			Utils::logr(buf, true);
		}
	}
	//ACCUMULATE("walkPos");

	return ret;
}

void AIUnit::restorePush(bool restore_self)
{
	for (unsigned i = 0; i < pushed_units.size(); ++i)
	{
		pushed_units[i]->restorePush(true);
	}
	pushed_units.clear();
	if (restore_self)
	{
		if (position_before_pushed.size())
		{
			//if (_unit_id == 38 || _unit_id == 52 || _unit_id == 41)
			//{
				//Utils::logAI("unit %d restored from %f,%f to %f,%f", _unit_id, _position.x, _position.y, position_before_pushed.back().x, position_before_pushed.back().y);
			//}
			_position = position_before_pushed.back();
			position_before_pushed.pop_back();
		}
	}
}

void AIUnit::confirmPush()
{
	for (unsigned i = 0; i < pushed_units.size(); ++i)
	{
		//pushed_units[i]->resetLastWalk(); //被推了的人就不用考虑之前甩头相关的数据了
		//if (pushed_units[i]->_unit_id == 38 || pushed_units[i]->_unit_id == 52 || pushed_units[i]->_unit_id == 41)
		//{
		//	Utils::logAI("unit %d confirm push (to %f,%f)", pushed_units[i]->_unit_id, pushed_units[i]->_position.x, pushed_units[i]->_position.y);
		//}
		pushed_units[i]->confirmPush();
	}
	position_before_pushed.clear();
	pushed_units.clear();
}

bool AIUnit::_tryWalk(cocos2d::Point& dest, float max_walk_length, bool enable_push, cocos2d::Vec2 original_dir, std::function<bool(cocos2d::Vec2, cocos2d::Vec2)> is_angle_ok, std::function<float(cocos2d::Vec2)> get_new_dir_length)
{
	std::vector<Collidable*> collidables;
	Terrain::getInstance()->getMeshByUnitRadius(_collide_radius)->getNearbyBorders(_position, max_walk_length, collidables);

	for (auto id_u : AI::getInstance()->_units) //单位们拷一遍到collidables中。以后如果单位特别多，也可以针对单位使用rtree。
	{
		if (id_u.second == this) continue; //不和自己碰撞
		collidables.push_back(id_u.second);
	}

	std::set<Collidable*> steered_collidables;

	for (;;) //其实很少转两次，一般最多转一次。但转两次的情况也是存在的，即在一个钝角处，先被边A转向，再被边B转向，结果还是小于指定的角度区间。先被单位A转向，再被单位B转向也是一样的
	{
		bool no_steer = true;

		//看看目前的方案是否有碰撞，如果有任意一个碰撞，那么尝试绕过它。
		for (auto c : collidables)
		{
			if (c->willCollide(this, dest))
			{
				bool priority_large = false;
				if (enable_push)
				{
					if (_priority > c->getPriority())
					{
						priority_large = true;
					}
					else if (_priority == c->getPriority())
					{
						//优先级相同的话已经肯定是单位了(目前还是可以这么假设的)
						AIUnit* other_unit = (AIUnit*)c;
						if (_unit_camp == other_unit->_unit_camp) //阵营相同才继续往下判priority2
						{
							if (_priority2 > other_unit->_priority2)
							{
								priority_large = true;
							}
							else if (_priority2 == other_unit->_priority2)
							{
								priority_large = _unit_id > other_unit->_unit_id; //对于优先级相等的情况，判unit_id
							}
						}
					}
				}
				if (priority_large && ((AIUnit*)c)->tryBePushed(dest, _collide_radius)) //如果unit的优先级比c高，那么c肯定不是个建筑，那么就强转成Unit来push
				{
					pushed_units.push_back((AIUnit*)c);
				}
				else //不能推动，那么要绕着走
				{
					//如果要决定绕过，不管绕不绕得过，之前推的都不用推了。就看绕的结果了
					restorePush(false);

					//如果碰撞了一个曾经绕过的东西，那就不用尝试再绕了，走不通
					if (steered_collidables.count(c))
					{
						return false;
					}

					steered_collidables.insert(c);

					no_steer = false;

					cocos2d::Vec2 new_dir;
					if (!c->getAdvisedNewDir(this, cocos2d::Vec2(_position, dest), new_dir)) //如果已经给不出建议方向了，那么说明没角度(正撞在墙上了)，说明走不通。如果是绕到这个方向再撞墙的，那也可以说明原方向走不通。
					{
						return false;
					}

					if (is_angle_ok(original_dir, new_dir))
					{
						float len;
						if (get_new_dir_length)
						{
							len = get_new_dir_length(new_dir);
						}
						else //没传get_new_dir_length方法的话就表示用的是max_walk_length，每次转向始终是走那么多
						{
							len = max_walk_length;
						}
						dest = _position + new_dir.getNormalized() * len;

						break; //break是为了进到外面那个for循环，再从头判
					}
					else //如果超过了的话，也说明走不通
					{
						return false;
					}
				}
			}
		}

		if (no_steer)
		{
			break;
		}
	}

	return true;
}

bool AIUnit::tryWalk(cocos2d::Point& dest, cocos2d::Vec2 original_dir)
{
	if (_tryWalk(dest, _max_walk_length, true, original_dir,
		[=](cocos2d::Vec2 original_dir, cocos2d::Vec2 new_dir) {
		//确认转向后和最初的向量没有超过90度。90度也许有点大，可调；现在是只是玩家单位有这个限定。其它单位绕其它单位的时候可以任意偏转原角度。当然绕建筑的时候还是有限定，在Border类里写了
		return (_unit_camp != "player" || _unit_camp != "opponent" || Geometry::deviateLessThan90(original_dir, new_dir));
	}))
	{
		confirmPush(); //走成功的话就confirmPush，其实就是把那个vector清掉，免得别人尝试push的时候又回滚了
		return true;
	}
	else
	{
		return false;
	}
}

bool AIUnit::tryBePushed(cocos2d::Point be_pushed_by_position, float be_pushed_by_radius)
	//priority在推人的过程中是有传递性的：只要发起推人的优先级高，就能推着一串人走（这串人自己的优先级无所谓，只要比发起人低）
{
	if (!AI::getInstance()->_ENABLE_PUSH)
	{
		return false;
	}

	float minimal_distance = be_pushed_by_radius + _collide_radius;
	cocos2d::Vec2 push_dir(be_pushed_by_position, _position);
	float current_distance = push_dir.getLength();

	assert(current_distance < minimal_distance);

	cocos2d::Point dest = _position + push_dir / current_distance * (minimal_distance - current_distance);

	float max_new_dir_length_sqr = sqr(minimal_distance) - sqr(current_distance);

	//if (_tryWalk(dest, sqrt(max_new_dir_length_sqr), be_pushed_by_priority, be_pushed_by_unit_id, push_dir, &Geometry::deviateLessThan90, //被推的单位就必须转向不能超过90度了，否则不自然
	if (_tryWalk(dest, sqrt(max_new_dir_length_sqr), false, push_dir, &Geometry::deviateLessThan90, //被推的单位就必须转向不能超过90度了，否则不自然
		[=](cocos2d::Vec2 new_dir) { 
		float cos_alpha = push_dir.dot(new_dir) / current_distance / new_dir.getLength();
		return -current_distance * cos_alpha + sqrt(sqr(current_distance * cos_alpha) + max_new_dir_length_sqr); //解了个方程
	}))
	{
		/*if (_unit_id == 38 || _unit_id == 52 || _unit_id == 41)
		{
			Utils::logAI("unit %d (may) be pushed from %f,%f to %f,%f; pushed_len: %f; max_len: %f", _unit_id, _position.x, _position.y, dest.x, dest.y, _position.distance(dest), sqrt(max_new_dir_length_sqr));
			if (AI::getInstance()->_current_frame == 273)
			{
				int x = 2;
			}
		}*/
		position_before_pushed.push_back(_position);
		_position = dest;
		_move_type = typeBePushed;
		return true;
	}
	else
	{
		//_tryWalk如果失败了，里面就已经restorePush了，外面没事了
		return false;
	}
}

cocos2d::ValueVector AIUnit::getGrassStatus()
{
	std::vector<std::string> grass_stepped_on_new;
	Terrain::getInstance()->grass_monitor.query(_position, grass_stepped_on_new);

	cocos2d::ValueVector ret;
	for (const auto& g : grass_stepped_on)
	{
		cocos2d::ValueMap grass;
		grass["gid"] = g;
		auto it_g = std::find(grass_stepped_on_new.begin(), grass_stepped_on_new.end(), g);
		if (it_g == grass_stepped_on_new.end())
		{
			grass["status"] = "leave";
			Utils::logAI("unit %d leave grass %s", _unit_id, g.c_str());
		}
		else
		{
			grass["status"] = "stay";
		}
		ret.push_back(cocos2d::Value(g));
	}

	for (const auto& g : grass_stepped_on_new)
	{
		if (std::find(grass_stepped_on.begin(), grass_stepped_on.end(), g) == grass_stepped_on.end())
		{
			cocos2d::ValueMap grass;
			grass["gid"] = g;
			grass["status"] = "enter";
			ret.push_back(cocos2d::Value(g));
			Utils::logAI("unit %d enter grass %s", _unit_id, g.c_str());
		}
	}

	grass_stepped_on = grass_stepped_on_new;

	return ret;
}

int AIUnit::getPriority() const
{
	return _priority;
}

bool AIUnit::willCollide(cocos2d::Point pos, float radius)
{
	cocos2d::Vec2 d = _position - pos;
	return (Fuzzy::_less(d.lengthSquared(), sqr(radius + _collide_radius))); //单位和单位允许相切，因为push单位的结果是刚好相切的
}

bool AIUnit::willCollide(AIUnit* unit, cocos2d::Point unit_new_pos)
{
	cocos2d::Vec2 d = _position - unit_new_pos;
	static auto debug_d = d;
	static auto debug_sqr = d.lengthSquared();
	static auto radius_sqr = sqr(unit->_collide_radius + _collide_radius);
	return (Fuzzy::_less(d.lengthSquared(), sqr(unit->_collide_radius + _collide_radius))); //单位和单位允许相切，因为push单位的结果是刚好相切的
}

bool AIUnit::getAdvisedNewDir(AIUnit* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir)
{
	cocos2d::Vec2 circle_center_dir(unit->_position, _position);
	if (Fuzzy::_greater(circle_center_dir.cross(old_dir), 0.0f))
	{
		new_dir = Geometry::anticlockwisePerpendicularVecToLine(circle_center_dir);
	}
	else
	{
		new_dir = Geometry::clockwisePerpendicularVecToLine(circle_center_dir);
	}
	return true;
}