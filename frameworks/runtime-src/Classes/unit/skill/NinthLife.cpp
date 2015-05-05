//
//  NinthLife.cpp
//  Boids
//
//  Created by chenyanjie on 5/5/15.
//
//

#include "NinthLife.h"
#include "../UnitNode.h"

using namespace cocos2d;

NinthLife::NinthLife() {
}

NinthLife::~NinthLife() {
    
}

NinthLife* NinthLife::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    NinthLife* ret = new NinthLife();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool NinthLife::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _buff_duration = data.at( "buff_duration" ).asValueVector().at( level - 1 ).asFloat();
    
    return true;
}

void NinthLife::updateFrame( float delta ) {
    
}

void NinthLife::begin() {
    ValueMap buff_data;
    buff_data["duration"] = Value( _buff_duration );
    buff_data["buff_type"] = Value( BUFF_TYPE_UNDEAD );
    buff_data["buff_name"] = Value( "LightShield" );
    buff_data["effect_resource"] = Value( "effects/kyle_skill_2" );
    
    UndeadBuff* buff = UndeadBuff::create( _owner, buff_data );
    _owner->addBuff( buff->getBuffId(), buff );
    buff->begin();
}

void NinthLife::end() {
    
}