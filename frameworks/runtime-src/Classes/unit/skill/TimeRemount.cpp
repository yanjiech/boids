//
//  TimeRemount.cpp
//  Boids
//
//  Created by chenyanjie on 7/14/15.
//
//

#include "TimeRemount.h"
#include "../../scene/BattleLayer.h"

using namespace cocos2d;

TimeRemount::TimeRemount() {
    
}

TimeRemount::~TimeRemount() {
    
}

TimeRemount* TimeRemount::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    TimeRemount* ret = new TimeRemount();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TimeRemount::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _hp_per = data.at( "hp" ).asValueVector().at( level - 1 ).asFloat();
    _mp_per = data.at( "mp" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = 0;
    
    return true;
}

void TimeRemount::updateFrame( float delta ) {
    
}

void TimeRemount::begin() {
    ValueMap buff_data;
    buff_data["buff_type"] = Value( BUFF_TYPE_RECOVER );
    buff_data["interval"] = Value( _interval );
    buff_data["hp"] = Value( _hp_per );
    buff_data["mp"] = Value( _mp_per );
    buff_data["duration"] = Value( _duration );
    buff_data["buff_name"] = Value( SKILL_NAME_TIME_REMOUNT );
    buff_data["effect_resource"] = Value( "effects/gandalf_skill_1" );
    buff_data["effect_scale_y"] = Value( 0.574712f );
    buff_data["buff_group"] = Value( (int)eBuffGroup::BuffGroupBuff );
    
    Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveUnitsByCamp( _owner->getTargetCamp() );
    for( auto unit : candidates ) {
        unit->removeAllDebuffs();
        Buff* buff = Buff::create( unit, buff_data );
        unit->addBuff( buff->getBuffId(), buff, true );
        buff->begin();
    }
}

void TimeRemount::end() {
}