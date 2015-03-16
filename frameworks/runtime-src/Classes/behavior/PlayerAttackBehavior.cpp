//
//  PlayerAttackBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/11/15.
//
//

#include "PlayerAttackBehavior.h"
#include "../unit/UnitNode.h"
#include "../scene/BattleLayer.h"

PlayerAttackBehavior::PlayerAttackBehavior( class UnitNode* unit_node ) : BehaviorBase( unit_node ) {
    
}

PlayerAttackBehavior::~PlayerAttackBehavior() {

}

PlayerAttackBehavior* PlayerAttackBehavior::create( class UnitNode* unit_node ) {
    PlayerAttackBehavior* ret = new PlayerAttackBehavior( unit_node );
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PlayerAttackBehavior::init() {
    if( !BehaviorBase::init() ) {
        return false;
    }
    return true;
}

bool PlayerAttackBehavior::behave( float delta ) {
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
        BattleLayer* battle_layer = _unit_node->getBattleLayer();
        cocos2d::Point control_dir = battle_layer->getControlLayer()->getJoyStickDirection();
        if( control_dir.x != 0 || control_dir.y != 0 ) {
            break;
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