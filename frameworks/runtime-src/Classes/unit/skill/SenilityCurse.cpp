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
    _atk = data.at( "atk" ).asValueVector().at( level - 1 ).asFloat();
    _def = data.at( "def" ).asValueVector().at( level - 1 ).asFloat();
    _mov_per = data.at( "mov" ).asFloat();
    _range = data.at( "range" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    
    return true;
}

void SenilityCurse::updateFrame( float delta ) {
    
}

void SenilityCurse::begin() {
    ValueMap buff_data;
    buff_data["buff_type"] = Value( BUFF_TYPE_CURSE );
    buff_data["buff_name"] = Value( SKILL_NAME_SENILITY_CURSE );
    buff_data["duration"] = Value( _duration );
    buff_data["effect_resource"] = Value( "effects/gandalf_skill_2" );
    buff_data["effect_pos"] = Value( (int)eBuffEffectPos::BuffEffectPosAboveHead );
    buff_data["effect_scale"] = Value( 0.5f );
//    buff_data["effect_layer"] = Value( 2 );
    buff_data["atk_fix"] = Value( _atk );
    buff_data["def_fix"] = Value( _def );
    buff_data["mov_per"] = Value( _mov_per );
    
    Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _range );
    
    for( auto unit : candidates ) {
        CurseBuff* buff = CurseBuff::create( unit, buff_data );
        unit->addBuff( buff->getBuffId(), buff );
        buff->begin();
    }
}

void SenilityCurse::end() {
    
}