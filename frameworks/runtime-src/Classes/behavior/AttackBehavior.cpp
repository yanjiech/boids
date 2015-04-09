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

AttackBehavior::AttackBehavior() {
    
}

AttackBehavior::~AttackBehavior() {
}

AttackBehavior* AttackBehavior::create( UnitNode* unit_node ) {
    AttackBehavior* ret = new AttackBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool AttackBehavior::init( UnitNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    return true;
}

bool AttackBehavior::behave( float delta ) {
    if( _unit_node->isDying() ) {
        return true;
    }
    if( _unit_node->isUnderControl() ) {
        return true;
    }
    if( _unit_node->isAttacking() || _unit_node->isCasting() ) {
        return true;
    }
    if( _unit_node->isHarmless() ) {
        return false;
    }
    if( _unit_node->isConcentrateOnWalk() ) {
        return false;
    }
    
    TargetNode* attack_target = _unit_node->getAttackTarget();
    if( attack_target != nullptr ) {
        if( _unit_node->canAttack( attack_target ) ) {
            _unit_node->attack( attack_target );
            return true;
        }
        else {
            _unit_node->setChasingTarget( attack_target );
            return false;
        }
    }
    else if( _unit_node->getChasingTarget() == nullptr && _unit_node->getSightGroup() != "" ) {
        cocos2d::Vector<UnitNode*> same_sight_group_units = _unit_node->getBattleLayer()->getAliveUnitsByCampAndSightGroup( _unit_node->getUnitCamp(), _unit_node->getSightGroup() );
        for( auto u : same_sight_group_units ) {
            if( u->getChasingTarget() != nullptr ) {
                _unit_node->setChasingTarget( u->getChasingTarget() );
                return false;
            }
        }
    }
    
    return false;
}