//
//  IceDebris.cpp
//  Boids
//
//  Created by chenyanjie on 5/29/15.
//
//

#include "IceDebris.h"

using namespace cocos2d;

IceDebris::IceDebris() {
    
}

IceDebris::~IceDebris() {
    
}

IceDebris* IceDebris::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    IceDebris* ret = new IceDebris();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool IceDebris::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    return true;
}

void IceDebris::updateFrame( float delta ) {
    
}

void IceDebris::begin() {
    
}

void IceDebris::end() {
    
}