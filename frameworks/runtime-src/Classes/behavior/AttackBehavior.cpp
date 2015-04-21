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

AttackBehavior* AttackBehavior::create( TargetNode* unit_node ) {
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

bool AttackBehavior::init( TargetNode* unit_node ) {
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
    if( unit_node->isCharging() ) {
        return false;
    }
    if( unit_node->isHarmless() ) {
        return false;
    }
    if( unit_node->isConcentrateOnWalk() ) {
        return false;
    }
    
    Point unit_pos = unit_node->getPosition();
    BattleLayer* battle_layer = unit_node->getBattleLayer();
    TargetNode* chasing_target = unit_node->getChasingTarget();
    float min_distance = ( chasing_target && chasing_target->isAttackable() ) ? chasing_target->getPosition().distance( unit_pos ) : INT_MAX;
    cocos2d::Vector<UnitNode*> candidates = battle_layer->getAliveOpponentsInRange( unit_node->getTargetCamp(), unit_pos, unit_pos, unit_node->getUnitData()->guard_radius );
    for( auto unit : candidates ) {
        if( unit->isAttackable() ) {
            float distance = unit_pos.distance( unit->getPosition() );
            if( distance < min_distance ) {
                chasing_target = unit;
                min_distance = distance;
            }
        }
    }
    
    cocos2d::Vector<TowerNode*> tower_candidates = battle_layer->getAliveTowersInRange( unit_node->getTargetCamp(), unit_pos, unit_pos, unit_node->getUnitData()->guard_radius );
    for( auto tower : tower_candidates ) {
        if( tower->isAttackable() ) {
            float distance = unit_pos.distance( tower->getPosition() );
            if( distance < min_distance ) {
                chasing_target = tower;
                min_distance = distance;
            }
        }
    }
    
    if( chasing_target != nullptr ) {
        if( unit_node->canAttack( chasing_target ) ) {
            unit_node->attack( chasing_target );
            return true;
        }
        else {
            unit_node->setChasingTarget( chasing_target );
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