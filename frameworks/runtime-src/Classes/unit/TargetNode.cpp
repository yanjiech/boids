//
//  TargetNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "TargetNode.h"

TargetNode::TargetNode() {
    
}

TargetNode::~TargetNode() {
    
}

bool TargetNode::init() {
    if( !Node::init() ) {
        return false;
    }
    this->setAttackable( true );
    return true;
}