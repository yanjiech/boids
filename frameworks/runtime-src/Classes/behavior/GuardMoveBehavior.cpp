//
//  GuardMoveBehavior.cpp
//  Boids
//
//  Created by chenyanjie on 4/22/15.
//
//

#include "GuardMoveBehavior.h"
#include "../scene/BattleLayer.h"
#include "../AI/Path.h"

using namespace cocos2d;

GuardMoveBehavior::GuardMoveBehavior() {
    
}

GuardMoveBehavior::~GuardMoveBehavior() {
    
}

GuardMoveBehavior* GuardMoveBehavior::create( class TargetNode* unit_node ) {
    GuardMoveBehavior* ret = new GuardMoveBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GuardMoveBehavior::init( TargetNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    
    return true;
}

bool GuardMoveBehavior::behave( float delta ) {
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _target_node );
    if( unit_node->isDying() ) {
        return true;
    }
    if( unit_node->isUnderControl() ) {
        return true;
    }
    
    float move_speed = unit_node->getUnitData()->move_speed;
    
    if( unit_node->getChasingTarget() != nullptr ) {
        Point last_pos = unit_node->getPosition();
        unit_node->findPathToPosition( unit_node->getChasingTarget()->getPosition() );
        unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    if( unit_node->isWalking() ) {
        unit_node->walkAlongWalkPath( move_speed * delta );
        return true;
    }
    if( !unit_node->needRelax() && !unit_node->isWalking() ) {
        BattleLayer* battle_layer = unit_node->getBattleLayer();
        UnitNode* guard_unit = unit_node->getGuardTarget();
        float guard_range = guard_unit->getUnitData()->guard_radius;
        float collide = guard_unit->getTargetData()->collide;
        Point guard_center = unit_node->getGuardCenter();
        Point wander_pos = Point::ZERO;
        for( int i = 0; i < 3; i++ ) {
            float r = collide + ( guard_range - collide ) * Utils::randomFloat();
            float angle = Utils::randomFloat() * M_PI;
            Point new_pos = Point( guard_center.x + cosf( angle ) * r, guard_center.y + sinf( angle ) * r );
            if( battle_layer->isPositionOK( new_pos, unit_node->getTargetData()->collide ) ) {
                wander_pos = new_pos;
                break;
            }
        }
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