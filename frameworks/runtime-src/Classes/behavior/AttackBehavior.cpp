//
//  AttackBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "AttackBehavior.h"
#include "../unit/UnitNode.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

AttackBehavior::AttackBehavior( UnitNode* unit_node ) : BehaviorBase( unit_node ) {
    
}

AttackBehavior::~AttackBehavior() {
}

AttackBehavior* AttackBehavior::create( UnitNode* unit_node ) {
    AttackBehavior* ret = new AttackBehavior( unit_node );
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool AttackBehavior::init() {
    if( !BehaviorBase::init() ) {
        return false;
    }
    return true;
}

bool AttackBehavior::behave( float delta ) {
    do {
        if( _unit_node->isDying() ) {
            return true;
        }
        if( _unit_node->isUnderControl() ) {
            return true;
        }
        if( _unit_node->isHarmless() ) {
            break;
        }
        if( _unit_node->isAttacking() || _unit_node->isCasting() ) {
            return true;
        }
        TargetNode* attack_target = _unit_node->getAttackTarget();
        if( attack_target == nullptr ) {
            break;
        }
        if( !_unit_node->canAttack( attack_target ) ) {
            _unit_node->setChasingTarget( attack_target );
            break;
        }
        
        _unit_node->attack( attack_target );
        return true;
    } while( true );
    
    return false;
}