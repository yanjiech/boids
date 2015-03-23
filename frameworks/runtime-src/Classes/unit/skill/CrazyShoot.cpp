//
//  CrazyShoot.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/20/15.
//
//

#include "CrazyShoot.h"


CrazyShoot::CrazyShoot() {
    
}

CrazyShoot::~CrazyShoot() {
    
}

CrazyShoot* CrazyShoot::create(  UnitNode* owner, const cocos2d::ValueMap& data ) {
    CrazyShoot* ret = new CrazyShoot();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CrazyShoot::init(  UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    return true;
}

void CrazyShoot::updateFrame( float delta ) {
    
}

void CrazyShoot::begin() {
    
}

void CrazyShoot::end() {
    
}