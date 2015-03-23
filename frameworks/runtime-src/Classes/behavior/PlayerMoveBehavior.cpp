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

PlayerMoveBehavior::PlayerMoveBehavior( UnitNode* unit_node ) : BehaviorBase( unit_node ) {
    
}

PlayerMoveBehavior::~PlayerMoveBehavior() {
    
}

PlayerMoveBehavior* PlayerMoveBehavior::create( UnitNode* unit_node ) {
    PlayerMoveBehavior* ret = new PlayerMoveBehavior( unit_node );
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PlayerMoveBehavior::init() {
    if( !BehaviorBase::init() ) {
        return false;
    }
    return true;
}

bool PlayerMoveBehavior::behave( float delta ) {
    if( _unit_node->isDying() ) {
        return true;
    }
    if( _unit_node->isUnderControl() ) {
        return true;
    }
    if( !_unit_node->isMovable() ) {
        return true;
    }
    BattleLayer* battle_layer = _unit_node->getBattleLayer();
    cocos2d::Point control_dir = battle_layer->getControlLayer()->getJoyStickDirection();
    float move_speed = _unit_node->getUnitData()->move_speed;
    if( _unit_node->shouldCatchUp() ) {
        Point last_pos = _unit_node->getPosition();
        _unit_node->findPathToPosition( battle_layer->getLeaderUnit()->getPosition() );
        _unit_node->walkAlongPath( move_speed * DEFAULT_CATCH_UP_SPEED_FACTOR * delta );
        return true;
    }
    if( control_dir.x != 0 || control_dir.y != 0 ) {
        Point new_pos = _unit_node->getPosition() + control_dir * delta * move_speed;
        _unit_node->walkTo( new_pos );
        return true;
    }
    if( _unit_node->getChasingTarget() == nullptr ) {
        if( _unit_node->getSightGroup() != "" ) {
            cocos2d::Vector<UnitNode*> same_sight_group_units = _unit_node->getBattleLayer()->getAliveUnitsByCampAndSightGroup( _unit_node->getUnitCamp(), _unit_node->getSightGroup() );
            for( auto u : same_sight_group_units ) {
                if( u->getChasingTarget() != nullptr ) {
                    _unit_node->setChasingTarget( u->getChasingTarget() );
                    break;
                }
            }
        }
    }
    if( _unit_node->getChasingTarget() ) {
        Point last_pos = _unit_node->getPosition();
        _unit_node->findPathToPosition( _unit_node->getChasingTarget()->getPosition() );
        _unit_node->walkAlongPath( move_speed * delta );
        return true;
    }
    return false;
}