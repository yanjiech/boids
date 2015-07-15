//
//  NinthLife.cpp
//  Boids
//
//  Created by chenyanjie on 5/5/15.
//
//

#include "NinthLife.h"
#include "../UnitNode.h"
#include "../../manager/AudioManager.h"

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
    _buff_duration = data.at( "duration" ).asFloat();
    _asp_per = data.at( "asp" ).asFloat();
    _cri = data.at( "cri" ).asValueVector().at( level - 1 ).asFloat();
    _elapse = 0;
    
    return true;
}

void NinthLife::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _buff_duration ) {
            this->end();
        }
    }
}

void NinthLife::begin() {
    SkillNode::begin();
    ValueMap buff_data;
    buff_data["duration"] = Value( _buff_duration );
    buff_data["buff_type"] = Value( BUFF_TYPE_BLESS );
    buff_data["buff_name"] = Value( SKILL_NAME_NINTH_LIFE );
    buff_data["effect_resource"] = Value( "effects/kyle_skill_2" );
    buff_data["effect_layer"] = Value( 1 );
    buff_data["atk_speed_per"] = Value( _asp_per );
    buff_data["cri_fix"] = Value( _cri );
    
    BlessBuff* buff = BlessBuff::create( _owner, buff_data );
    _owner->addBuff( buff->getBuffId(), buff );
    buff->begin();
    
    AudioManager::getInstance()->playEffect( "kyle/cast2_cont.mp3", true );
}

void NinthLife::end() {
    SkillNode::end();
    AudioManager::getInstance()->stopEffect( "kyle/cast2_cont.mp3" );
}