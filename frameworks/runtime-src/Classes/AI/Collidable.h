#ifndef __Boids_Collidable_h_
#define __Boids_Collidable_h_

#include "cocos2d.h"

class Collidable
{
public:
	virtual bool willCollide( cocos2d::Point pos, float radius) = 0;

	virtual bool willCollide( class TargetNode* unit, cocos2d::Point unit_new_pos) = 0;

	virtual bool getAdvisedNewDir( class UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir, bool visited = false ) = 0;

	virtual int getPriority() const = 0;
};

#endif