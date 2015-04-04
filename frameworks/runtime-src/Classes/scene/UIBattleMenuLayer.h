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
    
    static UIBattleMenuLayer* create( BattleLayer* battle_layer );
    virtual bool init( BattleLayer* battle_layer );
};

#endif /* defined(__Boids__UIBattleMenuLayer__) */
