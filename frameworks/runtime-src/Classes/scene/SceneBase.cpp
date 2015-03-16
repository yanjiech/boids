//
//  SceneBase.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "SceneBase.h"

SceneBase::SceneBase() {
    
}

SceneBase::~SceneBase() {
    
}

SceneBase* SceneBase::create() {
    SceneBase* ret = new SceneBase();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SceneBase::init() {
    if( !Scene::init() ) {
        return false;
    }
    return true;
}