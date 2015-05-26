//
//  TouchableLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#include "TouchableLayer.h"

using namespace cocos2d;

TouchableLayer::TouchableLayer() {
    
}

TouchableLayer::~TouchableLayer() {
    
}

bool TouchableLayer::init() {
    if( !Layer::init() ) {
        return false;
    }
    
    return true;
}

bool TouchableLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    return true;
}

void TouchableLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
}

void TouchableLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
}

void TouchableLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
}