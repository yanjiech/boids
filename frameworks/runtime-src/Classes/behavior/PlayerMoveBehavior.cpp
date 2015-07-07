//
//  PlayerMoveBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/10/15.
//
//

#include "PlayerMoveBehavior.h"
#include "../unit/UnitNode.h"
#include "../scene/BattleLayer.h"

#define DEFAULT_CATCH_UP_SPEED_FACTOR 1.5

using namespace cocos2d;

PlayerMoveBehavior::PlayerMoveBehavior()  {
    
}

PlayerMoveBehavior::~PlayerMoveBehavior() {
    
}

PlayerMoveBehavior* PlayerMoveBehavior::create( TargetNode* unit_node ) {
    PlayerMoveBehavior* ret = new PlayerMoveBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PlayerMoveBehavior::init( TargetNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    return true;
}

bool PlayerMoveBehavior::behave( float delta ) {
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _target_node );
    if( unit_node->isDying() ) {
        return true;
    }
    if( unit_node->isUnderControl() ) {
        return true;
    }
    BattleLayer* battle_layer = unit_node->getBattleLayer();
    cocos2d::Point control_dir = battle_layer->getControlLayer()->getJoyStickDirection();
    float move_speed = unit_node->getUnitData()->move_speed;
    
    if( unit_node->shouldCatchUp() && !unit_node->isCharging() ) {
        unit_node->setChasingTarget( nullptr );
        Point last_pos = unit_node->getPosition();
        unit_node->findPathToPosition( battle_layer->getLeaderUnit()->getPosition() );
        unit_node->walkAlongWalkPath( move_speed * DEFAULT_CATCH_UP_SPEED_FACTOR * delta );
        return true;
    }
    if( control_dir.x != 0 || control_dir.y != 0 ) {
        unit_node->setChasingTarget( nullptr );
        Point new_pos = unit_node->getPosition() + control_dir * delta * move_speed;
        unit_node->walkTo( new_pos );
        return true;
    }
    if( unit_node->isCharging() ) {
        return false;
    }
    if( unit_node->isAttacking() ) {
        return true;
    }
    if( unit_node->getChasingTarget() != nullptr ) {
        Point last_pos = unit_node->getPosition();
        unit_node->findPathToPosition( unit_node->getChasingTarget()->getPosition() );
        unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    return false;
}