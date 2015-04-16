//
//  IdleBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "IdleBehavior.h"
#include "../unit/UnitNode.h"

IdleBehavior::IdleBehavior() {
    
}

IdleBehavior::~IdleBehavior() {
    
}

IdleBehavior* IdleBehavior::create( UnitNode* unit_node ) {
    IdleBehavior* ret = new IdleBehavior();
    if( ret && ret->init( unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool IdleBehavior::init( UnitNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    return true;
}

bool IdleBehavior::behave( float delta ) {
    if( _unit_node->isDying() ) {
        return true;
    }
    _unit_node->changeUnitState( eUnitState::Idle );
    return true;
}