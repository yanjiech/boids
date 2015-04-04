//
//  Equipment.cpp
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#include "Equipment.h"

using namespace cocos2d;

Equipment::Equipment() {
    
}

Equipment::~Equipment() {
    
}

Equipment* Equipment::create( const cocos2d::ValueMap& data ) {
    Equipment* ret = new Equipment();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Equipment::init( const cocos2d::ValueMap& data ) {
    return true;
}