//
//  UIBattleFloatLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#ifndef __Boids__UIBattleFloatLayer__
#define __Boids__UIBattleFloatLayer__

#include "cocos2d.h"
#include "../interface/Updatable.h"

class BattleLayer;

class UIBattleFloatLayer : public cocos2d::Layer, public boids::Updatable {
private:
    BattleLayer* _battle_layer;
    
public:
    UIBattleFloatLayer();
    virtual ~UIBattleFloatLayer();
    
    static UIBattleFloatLayer* create( BattleLayer* battle_layer );
    virtual bool init( BattleLayer* battle_layer );
    
    virtual void updateFrame( float delta );
    
    void reset();
};

#endif /* defined(__Boids__UIBattleFloatLayer__) */
