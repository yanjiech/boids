//
//  DreadLord.cpp
//  Boids
//
//  Created by chenyanjie on 5/27/15.
//
//

#include "DreadLord.h"
#include "../UnitNode.h"
#include "../../Utils.h"

using namespace cocos2d;

DreadLord::DreadLord() {
    
}

DreadLord::~DreadLord() {
    
}

DreadLord* DreadLord::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    DreadLord* ret = new DreadLord();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DreadLord::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _extra_atk = data.at( "atk" ).asValueVector().at( level - 1 ).asFloat();
    _extra_dod = data.at( "dod" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    
    return true;
}

void DreadLord::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->end();
        }
    }
}

void DreadLord::begin() {
    SkillNode::begin();
    ValueMap buff_data;
    buff_data["buff_type"] = Value( BUFF_TYPE_BLESS );
    buff_data["buff_name"] = Value( SKILL_NAME_DREAD_LORD );
    buff_data["duration"] = Value( _duration );
    buff_data["atk_fix"] = Value( _extra_atk );
    buff_data["dod_fix"] = Value( _extra_dod );
    buff_data["effect_resource"] = Value( "effects/durarara_skill_2" );
    buff_data["effect_scale_x"] = Value( 1.74f );
    buff_data["atk_fix"] = Value( _extra_atk );
    buff_data["dod_fix"] = Value( _extra_dod );
    BlessBuff* buff = BlessBuff::create( _owner, buff_data );
    _owner->addBuff( buff->getBuffId(), buff );
    buff->begin();
    _owner->setUnitScale( _owner->getUnitScale() * 1.5f );
    _owner->endCast();
}

void DreadLord::end() {
    UnitData* unit_data = _owner->getUnitData();
    unit_data->atk -= _extra_atk;
    unit_data->dodge -= _extra_dod;
    _owner->setUnitScale( _owner->getUnitScale() / 1.5f );
    SkillNode::end();
}