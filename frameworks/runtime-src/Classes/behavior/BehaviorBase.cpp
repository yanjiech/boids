//
//  BehaviorBase.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "BehaviorBase.h"
#include "../unit/UnitNode.h"

BehaviorBase::BehaviorBase() : _target_node( nullptr ) {
}

BehaviorBase::~BehaviorBase() {}

bool BehaviorBase::init( TargetNode* target_node ) {
    this->setTargetNode( target_node );
    this->setEnabled( true );
    
    return true;
}

bool BehaviorBase::behave( float delta ) {
    return false;
}