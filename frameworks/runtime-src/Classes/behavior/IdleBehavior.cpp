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

IdleBehavior* IdleBehavior::create( TargetNode* unit_node ) {
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

bool IdleBehavior::init( TargetNode* unit_node ) {
    if( !BehaviorBase::init( unit_node ) ) {
        return false;
    }
    return true;
}

bool IdleBehavior::behave( float delta ) {
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _target_node );
    if( unit_node->isDying() ) {
        return true;
    }
    unit_node->changeUnitState( eUnitState::Idle );
    return true;
}