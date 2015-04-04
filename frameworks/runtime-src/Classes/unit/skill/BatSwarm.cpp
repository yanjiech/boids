//
//  BatSwarm.cpp
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#include "BatSwarm.h"

using namespace cocos2d;

BatSwarm::BatSwarm() {
    
}

BatSwarm::~BatSwarm() {
    
}

BatSwarm* BatSwarm::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    BatSwarm* ret = new BatSwarm();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BatSwarm::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    return true;
}

void BatSwarm::updateFrame( float delta ) {
    if( !_should_recycle ) {
        this->end();
    }
}

void BatSwarm::begin() {
    SkillNode::begin();
}

void BatSwarm::end() {
    SkillNode::end();
}