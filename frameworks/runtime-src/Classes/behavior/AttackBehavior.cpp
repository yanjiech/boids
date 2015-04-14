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
    if( unit_node->isHarmless() ) {
        return false;
    }
    if( unit_node->isConcentrateOnWalk() ) {
        return false;
    }
    
    TargetNode* attack_target = unit_node->getAttackTarget();
    if( attack_target != nullptr ) {
        if( unit_node->canAttack( attack_target ) ) {
            unit_node->attack( attack_target );
            return true;
        }
        else {
            unit_node->setChasingTarget( attack_target );
            return false;
        }
    }
    else if( unit_node->getChasingTarget() == nullptr && unit_node->getSightGroup() != "" ) {
        cocos2d::Vector<UnitNode*> same_sight_group_units = unit_node->getBattleLayer()->getAliveUnitsByCampAndSightGroup( unit_node->getTargetCamp(), unit_node->getSightGroup() );
        for( auto u : same_sight_group_units ) {
            if( u->getChasingTarget() != nullptr ) {
                unit_node->setChasingTarget( u->getChasingTarget() );
                return false;
            }
        }
    }
    
    return false;
}