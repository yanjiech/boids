//
//  SenilityCurse.cpp
//  Boids
//
//  Created by chenyanjie on 7/14/15.
//
//

#include "SenilityCurse.h"
#include "../../scene/BattleLayer.h"

using namespace cocos2d;

SenilityCurse::SenilityCurse() {
    
}

SenilityCurse::~SenilityCurse() {
    
}

SenilityCurse* SenilityCurse::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    SenilityCurse* ret = new SenilityCurse();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SenilityCurse::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _atk_per = data.at( "atk" ).asValueVector().at( level - 1 ).asFloat();
    _def_per = data.at( "def" ).asValueVector().at( level - 1 ).asFloat();
    _mov_per = data.at( "mov" ).asValueVector().at( level - 1 ).asFloat();
    
    _duration = data.at( "duration" ).asFloat();
    
    return true;
}

void SenilityCurse::updateFrame( float delta ) {
    
}

void SenilityCurse::begin() {
    
}

void SenilityCurse::end() {
    
}