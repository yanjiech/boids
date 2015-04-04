//
//  PlayerSkillBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/11/15.
//
//

#include "PlayerSkillBehavior.h"
#include "../unit/UnitNode.h"
#include "../Utils.h"

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
    _elapse = 0;
    return true;
}

bool PlayerSkillBehavior::behave( float delta ) {
    if( _unit_node->isDying() ) {
        return true;
    }
    if( _unit_node->isUnderControl() ) {
        return true;
    }
    if( _unit_node->isCasting() ) {
        return true;
    }
    
    if( _unit_node->getChasingTarget() != nullptr ) {
        cocos2d::Point target_pos = _unit_node->getChasingTarget()->getPosition();
        cocos2d::Point unit_pos = _unit_node->getPosition();
        float distance = target_pos.distance( unit_pos );
        _elapse += delta;
        if( _elapse >= 1.0f ) {
            _elapse = 0;
            int skill_count = _unit_node->getSkills().size();
            for( int i = 1; i < skill_count; i++ ) {
                float range = _unit_node->getSkillRangeById( i );
                if( ( range == 0 || distance <= range ) && _unit_node->isSkillReadyById( i ) && Utils::randomFloat() < 0.5 ) {
                    _unit_node->useSkill( i, _unit_node->getUnitDirection(), 0 );
                    return true;
                }
            }
        }
    }
    return false;
}