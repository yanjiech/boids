//
//  GuardAttackBehavior.cpp
//  Boids
//
//  Created by chenyanjie on 4/22/15.
//
//

#include "GuardAttackBehavior.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

GuardAttackBehavior::GuardAttackBehavior() {
    
}

GuardAttackBehavior::~GuardAttackBehavior() {
    
}

GuardAttackBehavior* GuardAttackBehavior::create( class TargetNode* unit_node ) {
    GuardAttackBehavior* ret = new GuardAttackBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GuardAttackBehavior::init( TargetNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    
    return true;
}

bool GuardAttackBehavior::behave( float delta ) {
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _target_node );
    
    if( unit_node->isDying() ) {
        return true;
    }
    if( unit_node->isUnderControl() ) {
        return true;
    }
    if( unit_node->isAttacking() || unit_node->isCasting() ) {
        return true;
    }
    if( unit_node->isCharging() ) {
        return false;
    }
    if( unit_node->isHarmless() ) {
        return false;
    }
    if( unit_node->isConcentrateOnWalk() ) {
        return false;
    }
    
    Point guard_center = unit_node->getGuardCenter();
    float guard_range = unit_node->getGuardRange();
    BattleLayer* battle_layer = unit_node->getBattleLayer();
    
    TargetNode* chasing_target = unit_node->getChasingTarget();
    if( chasing_target != nullptr && !Math::isPositionInRange( chasing_target->getPosition(), guard_center, guard_range ) ) {
        chasing_target = nullptr;
    }
    
    if( chasing_target == nullptr ) {
        Vector<UnitNode*> candidates = battle_layer->getAliveOpponentsInRange( unit_node->getTargetCamp(), guard_center, guard_range );
        int count = (int)candidates.size();
        if( count > 0 ) {
            int rand = Utils::randomNumber( count ) - 1;
            chasing_target = candidates.at( rand );
        }
    }
    
    unit_node->setChasingTarget( chasing_target );
    if( chasing_target ) {
        if( unit_node->canAttack( chasing_target ) ) {
            unit_node->attack( chasing_target );
            return true;
        }
    }
    
    return false;
}