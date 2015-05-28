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
    
    _touch_listener = EventListenerTouchOneByOne::create();
    _touch_listener->setSwallowTouches( true );
    _touch_listener->onTouchBegan = CC_CALLBACK_2( TouchableLayer::onTouchBegan, this );
    _touch_listener->onTouchMoved = CC_CALLBACK_2( TouchableLayer::onTouchMoved, this );
    _touch_listener->onTouchCancelled = CC_CALLBACK_2( TouchableLayer::onTouchCancelled, this );
    _touch_listener->onTouchEnded = CC_CALLBACK_2( TouchableLayer::onTouchEnded, this );
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority( _touch_listener, this );
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