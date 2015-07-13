#include "Border.h"
//#include "AI.h"
#include "Utils.h"
#include "../BoidsMath.h"
#include "../unit/UnitNode.h"

Border::Border(const cocos2d::Point& _src, const cocos2d::Point& _dst, const cocos2d::Point& _next_point) : src(_src), dst(_dst), vec(_src, _dst), next_vec(_dst, _next_point)
{
}

Border::Border( const Border& other ) {
    this->src = other.src;
    this->dst = other.dst;
    this->vec = other.vec;
    this->next_vec = other.next_vec;
}

int Border::getPriority() const //障碍物的优先级最高，以后如果有能被推动的障碍物再说。或者说能被推动的都是单位好了。
{
//	return AI::getInstance()->_BUILDING_PRIORITY;
    return 10000;
}

bool Border::willCollide(cocos2d::Point pos, float radius)
{
	//相切也算碰撞
	return (Fuzzy::_equalLess(Geometry::distanceSqrToLine(pos, src, dst), radius * radius));
}

bool Border::willCollide(TargetNode* unit, cocos2d::Point unit_new_pos )
{
	//相切也算碰撞；但起点和角重合的情况比较特殊（终点和角重合的情况好像没关系。画了一些图觉得是这样的。如果发现有问题要优先检查这里）
	//从障碍物里出到障碍物的情况要排除掉，这种情况是允许的。至于为什么进到障碍物里面了，这个方法就先不用考虑了，也许是误差吧

	float s, t;
	if (cocos2d::Vec2::isLineIntersect(unit->getPosition(), unit_new_pos, src, dst, &s, &t))
	{
		if (Fuzzy::_equal(s, 0.0f) && Fuzzy::_equal(t, 1.0f)) //遇到了起始点在转角的情况。要允许以转角为起点从障碍物里出来。
		{
			cocos2d::Vec2 unit_walk_step(unit->getPosition(), unit_new_pos);
			if (Fuzzy::_greater(vec.cross(next_vec), 0.0f)) //凹的角
			{
				//在其中一个向量的右侧就算碰撞（和一条平行的情况被包含在内了，算碰撞）
				return Fuzzy::_equalGreater(unit_walk_step.cross(vec), 0.0f) || Fuzzy::_equalGreater(unit_walk_step.cross(next_vec), 0.0f);
			}
			else //凸的角
			{
				//要在两个向量的右侧才算碰撞（和一条平行的情况被包含在内了，算碰撞）
				return Fuzzy::_equalGreater(unit_walk_step.cross(vec), 0.0f) && Fuzzy::_equalGreater(unit_walk_step.cross(next_vec), 0.0f);
			}
		}
		else
		{
			if (Fuzzy::_equalGreater(s, 0.0f) && Fuzzy::_equalLess(s, 1.0f) && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f))
			{
				return Fuzzy::_equalGreater(cocos2d::Vec2(unit->getPosition(), unit_new_pos).cross(vec), 0.0f); //平行着还是不能让他进的，否则沿着边平行的走，走着走着就走到建筑物里去了。。
			}
		}
	}
	else
	{
		//TODO 这里崩过一次，怀疑是端点重合导致
		return cocos2d::Vec2::isSegmentOverlap(unit->getPosition(), unit_new_pos, src, dst);
	}
	return false;
}

bool Border::getAdvisedNewDir(UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir, bool visited)
{
	cocos2d::Vec2 perpendicular_towards_outside = Geometry::clockwisePerpendicularVecToLine(vec);
	if (Fuzzy::_greater(old_dir.cross(perpendicular_towards_outside), 0.0f))
	{
		new_dir = Geometry::clockwiseRotate1(-vec); //再额外转一度，免得正好和边平行
	}
	else
	{
		new_dir = Geometry::anticlockwiseRotate1(vec);
	}

//	return Geometry::deviateLessThan90(old_dir, new_dir);
    return true;
}

segment Border::getBoostSeg()
{
	return segment(Utils::toPt(src), Utils::toPt(dst));
}