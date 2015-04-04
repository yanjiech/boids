//
//  BehaviorBase.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "BehaviorBase.h"
#include "../unit/UnitNode.h"

BehaviorBase::BehaviorBase( UnitNode* unit_node ) : _unit_node( unit_node ) {
}

BehaviorBase::~BehaviorBase() {}

bool BehaviorBase::init() {
    if( _unit_node == nullptr ) {
        return false;
    }
    return true;
}

bool BehaviorBase::behave( float delta ) {
    return false;
}