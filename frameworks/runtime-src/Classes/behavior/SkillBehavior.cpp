//
//  SkillBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "SkillBehavior.h"

SkillBehavior::SkillBehavior( UnitNode* unit_node ) : BehaviorBase( unit_node ) {
    
}

SkillBehavior::~SkillBehavior() {
    
}

SkillBehavior* SkillBehavior::create( UnitNode* unit_node ) {
    SkillBehavior* ret = new SkillBehavior( unit_node );
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SkillBehavior::init() {
    if( !BehaviorBase::init() ) {
        return false;
    }
    return true;
}

bool SkillBehavior::behave( float delta ) {
    return false;
}