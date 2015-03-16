//
//  Skill.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#include "Skill.h"

using namespace cocos2d;

Skill::Skill() {
    
}

Skill::~Skill() {
    
}

Skill* Skill::create( const std::string& name ) {
    Skill* ret = new Skill();
    if( ret && ret->init( name ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Skill::init( const std::string& namae ) {
    return true;
}