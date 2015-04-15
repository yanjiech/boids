//
//  HuntingIntuition.cpp
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#include "HuntingIntuition.h"
#include "../UnitNode.h"

using namespace cocos2d;

HuntingIntuition::HuntingIntuition() {
    
}

HuntingIntuition::~HuntingIntuition() {
    
}

HuntingIntuition* HuntingIntuition::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    HuntingIntuition* ret = new HuntingIntuition();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool HuntingIntuition::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    _buff_duration = data.at( "buff_duration" ).asFloat();
    _cri_up = data.at( "cri_up" ).asFloat();
    _hit_up = data.at( "hit_up" ).asFloat();
    
    return true;
}

void HuntingIntuition::updateFrame( float delta ) {
    
}

void HuntingIntuition::begin() {
    
}

void HuntingIntuition::end() {
    ValueMap data;
    data["duration"] = Value( _buff_duration );
    data["buff_type"] = Value( BUFF_TYPE_ATTRIBUTE );
    data["buff_name"] = Value( "HuntingIntuition" );
    
    ElementData* unit_data = _owner->getTargetData();
    
    ValueMap attributes;
    attributes["cri"] = Value( unit_data->critical * _cri_up );
    attributes["hit"] = Value( unit_data->hit * _hit_up );
    data["attributes"] = Value( attributes );
    
    AttributeBuff* buff = AttributeBuff::create( _owner, data );
    _owner->addBuff( buff->getBuffId(), buff );
    buff->begin();
}