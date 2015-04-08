#include "Path.h"
#include "../BoidsMath.h"
#include "../Utils.h"

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

//const Path& Path::operator = ( const Path& other ) {
//    if( this == &other ) {
//        return *this;
//    }
//    this->steps = other.steps;
//    this->validity_frame = other.validity_frame;
//    return *this;
//}

Path::Path() :
customized_is_reach_destination(nullptr),
path_released_callback(nullptr)
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

Path* Path::create( int valid_frame ) {
    Path* ret = new Path();
    if( ret && ret->init( valid_frame ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Path::init( int valid_frame ) {
    validity_frame = valid_frame;
    return true;
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

Path::~Path()
{
}