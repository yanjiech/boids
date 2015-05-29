//
//  SnowStorm.cpp
//  Boids
//
//  Created by chenyanjie on 5/29/15.
//
//

#include "SnowStorm.h"

using namespace cocos2d;

SnowStorm::SnowStorm() {
    
}

SnowStorm::~SnowStorm() {
    
}

SnowStorm* SnowStorm::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    SnowStorm* ret = new SnowStorm();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SnowStorm::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    return true;
}

void SnowStorm::updateFrame( float delta ) {
    
}

void SnowStorm::begin() {
    SkillNode::begin();
}

void SnowStorm::end() {
    SkillNode::end();
}