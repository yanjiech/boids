//
//  MoveBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "MoveBehavior.h"
#include "../unit/UnitNode.h"
#include "../scene/BattleLayer.h"
#include "../AI/Path.h"

using namespace cocos2d;

MoveBehavior::MoveBehavior() {
    
}

MoveBehavior::~MoveBehavior() {
    
}

MoveBehavior* MoveBehavior::create( UnitNode* unit_node ) {
    MoveBehavior* ret = new MoveBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MoveBehavior::init( UnitNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    return true;
}

bool MoveBehavior::behave( float delta ) {
    if( _unit_node->isDying() ) {
        return true;
    }
    if( _unit_node->isUnderControl() ) {
        return true;
    }
    
    float move_speed = _unit_node->getUnitData()->move_speed;
    
    if( _unit_node->getChasingTarget() ) {
        Point last_pos = _unit_node->getPosition();
        _unit_node->findPathToPosition( _unit_node->getChasingTarget()->getPosition() );
        _unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    if( _unit_node->getTourPath() ) {
        _unit_node->walkAlongTourPath( move_speed * delta );
        return true;
    }
    if( _unit_node->isWalking() ) {
        _unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    if( !_unit_node->needRelax() && !_unit_node->isWalking() ) {
        Point wander_pos = _unit_node->getNextWanderPos();
        if( wander_pos.x != 0 || wander_pos.y != 0 ) {
            Path* path = Path::create( INT_MAX );
            path->steps.push_back( wander_pos );
            _unit_node->setWalkPath( path );
            _unit_node->walkAlongWalkPath( move_speed * delta );
            return true;
        }
    }
    return false;
}