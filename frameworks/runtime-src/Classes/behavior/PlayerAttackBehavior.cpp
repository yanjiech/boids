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

PlayerAttackBehavior::PlayerAttackBehavior()  {
    
}

PlayerAttackBehavior::~PlayerAttackBehavior() {

}

PlayerAttackBehavior* PlayerAttackBehavior::create( class UnitNode* unit_node ) {
    PlayerAttackBehavior* ret = new PlayerAttackBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PlayerAttackBehavior::init( UnitNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    return true;
}

bool PlayerAttackBehavior::behave( float delta ) {
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _target_node );
    if( unit_node->isDying() ) {
        return true;
    }
    if( unit_node->isUnderControl() ) {
        return true;
    }
    if( unit_node->isCasting() ) {
        return true;
    }
    if( unit_node->isAttacking() ) {
        return false;
    }
    if( unit_node->isHarmless() ) {
        return false;
    }
    BattleLayer* battle_layer = unit_node->getBattleLayer();
    cocos2d::Point control_dir = battle_layer->getControlLayer()->getJoyStickDirection();
    if( control_dir.x != 0 || control_dir.y != 0 ) {
        return false;
    }
    TargetNode* attack_target = unit_node->getAttackTarget();
    if( attack_target == nullptr ) {
        return false;
    }
    if( !unit_node->canAttack( attack_target ) ) {
        unit_node->setChasingTarget( attack_target );
        return false;
    }
    
    unit_node->attack( attack_target );
    return true;
}