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
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _target_node );
    if( unit_node->isDying() ) {
        return true;
    }
    if( unit_node->isUnderControl() ) {
        return true;
    }
    
    float move_speed = unit_node->getUnitData()->move_speed;
    
    if( unit_node->getChasingTarget() ) {
        Point last_pos = unit_node->getPosition();
        unit_node->findPathToPosition( unit_node->getChasingTarget()->getPosition() );
        unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    if( unit_node->getTourPath() ) {
        unit_node->walkAlongTourPath( move_speed * delta );
        return true;
    }
    if( unit_node->isWalking() ) {
        unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    if( !unit_node->needRelax() && !unit_node->isWalking() ) {
        Point wander_pos = unit_node->getNextWanderPos();
        if( wander_pos.x != 0 || wander_pos.y != 0 ) {
            Path* path = Path::create( INT_MAX );
            path->steps.push_back( wander_pos );
            unit_node->setWalkPath( path );
            unit_node->walkAlongWalkPath( move_speed * delta );
            return true;
        }
    }
    return false;
}