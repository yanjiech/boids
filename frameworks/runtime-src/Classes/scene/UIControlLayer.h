//
//  UIControlLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__UIControlLayer__
#define __Boids__UIControlLayer__

#include "cocos2d.h"

class JoyStick : public cocos2d::Node {
private:
    cocos2d::Sprite* _back_sprite;
    cocos2d::Sprite* _front_sprite;
    
    float _radius;
    
public:
    static JoyStick* create( const std::string& back_name, const std::string& front_name, float radius );
    virtual bool init( const std::string& back_name, const std::string& front_name, float radius );
    
    cocos2d::Point getDirection();
    
    void setFrontPosition( const cocos2d::Point& pos );
    
    void reset();
};

class UIControlLayer : public cocos2d::Layer {
private:
    bool _is_touch_began;
    
    JoyStick* _joystick;
    
    class BattleLayer* _battle_layer;
    
public:
    UIControlLayer();
    ~UIControlLayer();
    
    static UIControlLayer* create( class BattleLayer* battle_layer );
    
    virtual bool init( class BattleLayer* battle_layer );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
    
    cocos2d::Point getJoyStickDirection();
};

#endif /* defined(__Boids__UIControlLayer__) */
