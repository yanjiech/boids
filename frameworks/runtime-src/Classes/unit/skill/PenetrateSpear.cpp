//
//  PenetrateSpear.cpp
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#include "PenetrateSpear.h"
#include "../../scene/BattleLayer.h"

using namespace cocos2d;

PenetrateSpear::PenetrateSpear() {
    
}

PenetrateSpear::~PenetrateSpear() {
    
}

PenetrateSpear* PenetrateSpear::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    PenetrateSpear* ret = new PenetrateSpear();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PenetrateSpear::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    return true;
}

void PenetrateSpear::updateFrame( float delta ) {
    if( !_should_recycle ) {
        BattleLayer* battle_layer = _owner->getBattleLayer();
        Point control_dir = battle_layer->getControlLayer()->getJoyStickDirection();
        if( control_dir.x == 0 && control_dir.y == 0 ) {
            
        }
        else {
            
        }
    }
}

void PenetrateSpear::begin() {
    SkillNode::begin();
}

void PenetrateSpear::end() {
    _owner->endCast();
    SkillNode::end();
}