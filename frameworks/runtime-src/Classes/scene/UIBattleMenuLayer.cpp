//
//  UIBattleMenuLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/5/15.
//
//

#include "UIBattleMenuLayer.h"

UIBattleMenuLayer::UIBattleMenuLayer() {
    
}

UIBattleMenuLayer::~UIBattleMenuLayer() {
    
}

UIBattleMenuLayer* UIBattleMenuLayer::create( BattleLayer* battle_layer ) {
    UIBattleMenuLayer* ret = new UIBattleMenuLayer();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleMenuLayer::init( BattleLayer* battle_layer ) {
    _battle_layer = battle_layer;
    return true;
}