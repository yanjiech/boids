//
//  UIBattleMenuLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/5/15.
//
//

#ifndef __Boids__UIBattleMenuLayer__
#define __Boids__UIBattleMenuLayer__

#include "cocos2d.h"

class BattleLayer;

class UIBattleMenuLayer : public cocos2d::Layer {
private:
    BattleLayer* _battle_layer;
    
public:
    UIBattleMenuLayer();
    virtual ~UIBattleMenuLayer();
    
    static UIBattleMenuLayer* create( BattleLayer* battle_layer, const std::string& result );
    virtual bool init( BattleLayer* battle_layer, const std::string& result );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEned( cocos2d::Touch* touch, cocos2d::Event* event );
    
    void onConfirmTouched( cocos2d::Ref* sender );
};

#endif /* defined(__Boids__UIBattleMenuLayer__) */
