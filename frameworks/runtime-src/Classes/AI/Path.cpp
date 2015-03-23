#include "Path.h"
#include "BoidsMath.h"
#include "Utils.h"
//#include "AI.h"

Path::Path(const std::string& steps_serialization)
{
	//这段代码在BoidsPolygonEx里也用了，再重用就把他提到Utils里去。。
	std::vector<std::string> split_output;
	Utils::split(steps_serialization, split_output, ' ');
	for (unsigned int i = 0; i < split_output.size(); ++i)
	{
		if (split_output[i].empty()) continue; //如果有空的说明出现了连续空格，认为是多打了，忽略
		float x, y;
		sscanf(split_output[i].c_str(), "(%f,%f)", &x, &y);
		steps.push_back(cocos2d::Point(x, y));
	}
}

std::string Path::serialize()
{
	std::ostringstream os;
	for (auto pt : steps)
	{
		os << "(" << pt.x << "," << pt.y << ") ";
	}
	std::string ret = os.str();
	ret.pop_back();
	return ret;
}

bool Path::operator == (const class Path& other)
{
	if (steps.size() != other.steps.size())
	{
		return false;
	}
	for (unsigned int i = 0; i < steps.size(); ++i)
	{
		if (steps[i].fuzzyEquals(other.steps[i], FUZZY_THRESHOLD)) continue;
		return false;
	}
	return true;
}

const Path& Path::operator = ( const Path& other ) {
    if( this == &other ) {
        return *this;
    }
    this->steps = other.steps;
    this->validity_frame = other.validity_frame;
    return *this;
}

Path::Path() :
validity_frame( INT_MAX ), customized_is_reach_destination(nullptr), path_released_callback(nullptr)
{
    
}

Path::Path(int _validity_frame) : 
validity_frame( _validity_frame ), customized_is_reach_destination(nullptr), path_released_callback(nullptr)
{
}

void Path::paint(cocos2d::Point from, cocos2d::DrawNode* node, cocos2d::Color4F c)
{
	if (steps.empty())
	{
		cocos2d::log("path: nothing to paint.");
	}
	else
	{
		node->drawLine(from, steps.back(), c);
		for (unsigned i = 1; i != steps.size(); ++i)
		{
			node->drawLine(steps[i - 1], steps[i], c);
		}
		for (unsigned i = 0; i != steps.size(); ++i)
		{
			node->drawDot(steps[i], 6, cocos2d::Color4F::RED);
		}
	}
}

bool Path::isOutdated()
{
    return true;
//	return AI::getInstance()->_current_frame >= validity_frame;
}

bool Path::isPathEnd()
{
	return steps.empty();
}

cocos2d::Point Path::walkFrame(cocos2d::Point current_pos, int max_walk_length)
{
	assert(steps.size()); //如果steps里面没东西了就不应该进来了

	cocos2d::Point new_pos;
	cocos2d::Vec2 vector_to_step_point(current_pos, steps.back());
	float distance = vector_to_step_point.lengthSquared();
	if (distance <= max_walk_length * max_walk_length)
	{
		new_pos = steps.back(); //加了Boids算法之后我估计这种情况会有点问题，再改
		steps.pop_back();
	}
	else
	{
		new_pos = current_pos + vector_to_step_point / sqrt(distance) * max_walk_length;
	}

	if (customized_is_reach_destination && customized_is_reach_destination(new_pos))
	{
		steps.clear();
		//Utils::logAI("customized_reach_destination");
	}

	return new_pos;
}

Path::~Path()
{
	if (path_released_callback)
	{
		path_released_callback();
	}
}