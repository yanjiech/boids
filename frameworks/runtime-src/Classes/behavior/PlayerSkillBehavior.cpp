//
//  PlayerSkillBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/11/15.
//
//

#include "PlayerSkillBehavior.h"
#include "../unit/UnitNode.h"

#define DEFAULT_CATCH_UP_DISTANCE 80.0

PlayerSkillBehavior::PlayerSkillBehavior( class UnitNode* unit_node ) : BehaviorBase( unit_node ) {

}

PlayerSkillBehavior::~PlayerSkillBehavior() {
    
}

PlayerSkillBehavior* PlayerSkillBehavior::create( class UnitNode* unit_node ) {
    PlayerSkillBehavior* ret = new PlayerSkillBehavior( unit_node );
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PlayerSkillBehavior::init() {
    if( !BehaviorBase::init() ) {
        return false;
    }
    return true;
}

bool PlayerSkillBehavior::behave( float delta ) {
    return false;
}