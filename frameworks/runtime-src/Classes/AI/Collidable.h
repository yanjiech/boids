﻿#ifndef __Boids_Collidable_h_
#define __Boids_Collidable_h_

#include "cocos2d.h"

class Collidable
{
public:
	virtual bool willCollide(cocos2d::Point pos, float radius) = 0;
	bool willCollide(class UnitNode* unit);

	virtual bool willCollide(class UnitNode* unit, cocos2d::Point unit_new_pos) = 0;

	virtual bool getAdvisedNewDir(class UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir) = 0;

	//数字越大优先级越高，越不能被推动。
	virtual int getPriority() const = 0;
};

#endif