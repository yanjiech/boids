//
//  UIControlLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "UIControlLayer.h"

#define JOYSTICK_WHEEL_RADIUS 128.0

using namespace cocos2d;

JoyStick* JoyStick::create( const std::string& back_name, const std::string& front_name, float radius ) {
    JoyStick* ret = new JoyStick();
    if( ret && ret->init( back_name, front_name, radius ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool JoyStick::init( const std::string& back_name, const std::string& front_name, float radius ) {
    _back_sprite = Sprite::createWithSpriteFrameName( back_name );
    this->addChild( _back_sprite, 1 );
    
    _front_sprite = Sprite::createWithSpriteFrameName( front_name );
    this->addChild( _front_sprite, 2 );
    
    _radius = radius;
    
    return true;
}

cocos2d::Point JoyStick::getDirection() {
    Point delta = _front_sprite->getPosition();
    delta.normalize();
    return delta;
}

void JoyStick::setFrontPosition( const cocos2d::Point& pos ) {
    Point delta = pos - this->getPosition();
    float delta_length = delta.getLength();
    if( delta_length > _radius ) {
        delta *= ( _radius / delta_length );
    }
    _front_sprite->setPosition( delta );
}

void JoyStick::reset() {
    _front_sprite->setPosition( Point::ZERO );
}

UIControlLayer::UIControlLayer() {
    
}

UIControlLayer::~UIControlLayer() {
    
}

UIControlLayer* UIControlLayer::create() {
    UIControlLayer* ret = new UIControlLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIControlLayer::init() {
    if( !Layer::init() ) {
        return false;
    }
    
    _joystick = JoyStick::create( "joystick_back.png", "joystick_front.png", JOYSTICK_WHEEL_RADIUS );
    this->addChild( _joystick );
    _joystick->setVisible( false );
    
    _is_touch_began = false;
    auto touch_event_listener = EventListenerTouchOneByOne::create();
    touch_event_listener->setSwallowTouches( true );
    touch_event_listener->onTouchBegan = CC_CALLBACK_2( UIControlLayer::onTouchBegan, this );
    touch_event_listener->onTouchMoved = CC_CALLBACK_2( UIControlLayer::onTouchMoved, this );
    touch_event_listener->onTouchCancelled = CC_CALLBACK_2( UIControlLayer::onTouchCancelled, this );
    touch_event_listener->onTouchEnded = CC_CALLBACK_2( UIControlLayer::onTouchEnded, this );
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_event_listener, this );
    
    return true;
}

bool UIControlLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( !_is_touch_began ) {
        Point touch_down_pos = this->convertTouchToNodeSpace( touch );
        _joystick->setVisible( true );
        _joystick->setPosition( touch_down_pos );
        _is_touch_began = true;
        return true;
    }
    return false;
}

void UIControlLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began ) {
        Point touch_pos = this->convertTouchToNodeSpace( touch );
        _joystick->setFrontPosition( touch_pos );
    }
}

void UIControlLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began ) {
        _joystick->setVisible( false );
        _is_touch_began = false;
        _joystick->reset();
    }
}

void UIControlLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began ) {
        _joystick->setVisible( false );
        _is_touch_began = false;
        _joystick->reset();
    }
}

cocos2d::Point UIControlLayer::getJoyStickDirection() {
    if( _is_touch_began ) {
        return _joystick->getDirection();
    }
    else {
        return Point::ZERO;
    }
}