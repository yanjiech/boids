//
//  SkillBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "SkillBehavior.h"
#include "../unit/UnitNode.h"
#include "../Utils.h"

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
    _elapse = 0;
    return true;
}

bool SkillBehavior::behave( float delta ) {
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
            for( int i = 0; i < skill_count; i++ ) {
                float range = _unit_node->getSkillRangeById( i );
                if( ( distance <= range || range == 0 ) && _unit_node->isSkillReadyById( i ) && Utils::randomFloat() < 0.5 ) {
                    _unit_node->useSkill( i, _unit_node->getUnitDirection(), 0 );
                    return true;
                }
            }
        }
    }
    
    return false;
}