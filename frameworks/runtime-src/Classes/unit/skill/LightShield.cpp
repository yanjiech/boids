//
//  LightShield.cpp
//  Boids
//
//  Created by chenyanjie on 3/25/15.
//
//

#include "LightShield.h"
#include "../UnitNode.h"

using namespace cocos2d;

LightShield::LightShield() {
    
}

LightShield::~LightShield() {
    
}

LightShield* LightShield::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    LightShield* ret = new LightShield();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool LightShield::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _absorb = data.at( "hp" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    return true;
}

void LightShield::updateFrame( float delta ) {
}

void LightShield::begin() {
    ValueMap data;
    data["duration"] = Value( _duration );
    data["absorb"] = Value( _absorb );
    data["buff_type"] = Value( BUFF_TYPE_SHIELD );
    data["buff_name"] = Value( "LightShield" );
    data["effect_resource"] = Value( "effects/saber_skill_2" );
    data["effect_scale"] = Value( 1.0f );
    ShieldBuff* buff = ShieldBuff::create( _owner, data );
    _owner->addBuff( buff->getBuffId(), buff );
    buff->begin();
}

void LightShield::end() {
}