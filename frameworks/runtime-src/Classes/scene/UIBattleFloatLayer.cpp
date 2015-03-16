//
//  UIBattleFloatLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#include "UIBattleFloatLayer.h"
#include "BattleLayer.h"

using namespace cocos2d;

UIBattleFloatLayer::UIBattleFloatLayer() {
    
}

UIBattleFloatLayer::~UIBattleFloatLayer() {
    
}

UIBattleFloatLayer* UIBattleFloatLayer::create( BattleLayer* battle_layer ) {
    UIBattleFloatLayer* ret = new UIBattleFloatLayer();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleFloatLayer::init( BattleLayer* battle_layer ) {
    if( !Layer::init() ) {
        return false;
    }
    
    return true;
}

void UIBattleFloatLayer::updateFrame( float delta ) {
    
}

void UIBattleFloatLayer::reset() {
    
}