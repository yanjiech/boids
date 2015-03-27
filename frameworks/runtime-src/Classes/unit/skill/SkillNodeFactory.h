//
//  SkillNodeFactory.h
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#ifndef __Boids__SkillNodeFactory__
#define __Boids__SkillNodeFactory__

#include "WrathOfThunder.h"
#include "LightningChain.h"
#include "BarrageOfArrow.h"
#include "CrazyShoot.h"
#include "GiantSword.h"
#include "LightShield.h"

class SkillNodeFactory {
public:
    static SkillNode* createSkillNode( const std::string& name, UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
};

#endif /* defined(__Boids__SkillNodeFactory__) */