#ifndef __Boids_Border_h
#define __Boids_Border_h

#include "Collidable.h"
#include "BoostGeometryDef.h"

class Border : public Collidable
{
public:
	Border(const cocos2d::Point& _src, const cocos2d::Point& _dst, const cocos2d::Point& _next_point);
    
    Border( const Border& other );

	bool willCollide(cocos2d::Point pos, float radius);

	bool willCollide(class UnitNode* unit, cocos2d::Point unit_new_pos);

	bool getAdvisedNewDir(class UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir);

	int getPriority() const;

	cocos2d::Point src, dst;
	cocos2d::Vec2 vec, next_vec; //之所以要多存一个next_vec，就是要处理以角为起点进入多边形的情况：只存两个店的话信息不足

	segment getBoostSeg();
};

#endif