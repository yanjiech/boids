//
//  IdleBehavior.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "IdleBehavior.h"
#include "../unit/UnitNode.h"

IdleBehavior::IdleBehavior( UnitNode* unit_node ) : BehaviorBase( unit_node ) {
    
}

IdleBehavior::~IdleBehavior() {
    
}

IdleBehavior* IdleBehavior::create( UnitNode* unit_node ) {
    IdleBehavior* ret = new IdleBehavior( unit_node );
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool IdleBehavior::init() {
    if( !BehaviorBase::init() ) {
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