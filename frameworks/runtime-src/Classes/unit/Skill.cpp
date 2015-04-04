//
//  Skill.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#include "Skill.h"
#include "../manager/ResourceManager.h"
#include "../util/CocosUtils.h"
#include "./skill/SkillNodeFactory.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

Skill::Skill() {
    
}

Skill::~Skill() {
    
}

Skill* Skill::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    Skill* ret = new Skill();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Skill::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    _owner = owner;
    this->setSkillName( data.at( "name" ).asString() );
    this->setSkillLevel( data.at( "level" ).asInt() );
    this->setSkillState( eSkillState::SkillStateLoading );
    _elapse = 0;
    
    _skill_data = ResourceManager::getInstance()->getSkillData( _skill_name );
    _skill_data["level"] = Value( _level );
    _full_cd = _skill_data.at( "cd" ).asFloat();
    
    return true;
}

void Skill::updateFrame( float delta ) {
    if( _state == eSkillState::SkillStateLoading ) {
        _elapse += delta;
        if( _elapse >= _full_cd ) {
            _elapse = _full_cd;
            this->setSkillState( eSkillState::SkillStateReady );
        }
    }
}

void Skill::activate( const cocos2d::ValueMap& params ) {
    _elapse = 0;
    this->setSkillState( eSkillState::SkillStateCasting );
    SkillNode* skill_node = SkillNodeFactory::createSkillNode( _skill_name, _owner, _skill_data, params );
    skill_node->begin();
}

void Skill::reload() {
    _elapse = 0;
    this->setSkillState( eSkillState::SkillStateLoading );
}

bool Skill::isSkillReady() {
    return _state == eSkillState::SkillStateReady;
}

float Skill::getSkillCD() {
    return _elapse / _full_cd;
}

float Skill::getSkillRadius() {
    auto itr = _skill_data.find( "radius" );
    if( itr != _skill_data.end() ) {
        return itr->second.asFloat();
    }
    return 0;
}

float Skill::getSkillRange() {
    auto itr = _skill_data.find( "range" );
    if( itr != _skill_data.end() ) {
        return itr->second.asFloat();
    }
    return 0;
}

float Skill::getSkillMaxRange() {
    auto itr = _skill_data.find( "max_range" );
    if( itr != _skill_data.end() ) {
        return itr->second.asFloat();
    }
    return 0;
}

float Skill::getSkillMinRange() {
    auto itr = _skill_data.find( "min_range" );
    if( itr != _skill_data.end() ) {
        return itr->second.asFloat();
    }
    return 0;
}

std::string Skill::getSkillHintType() {
    return _skill_data.at( "hint" ).asString();
}

std::string Skill::getChargingEffect() {
    auto itr = _skill_data.find( "charging_effect" );
    if( itr != _skill_data.end() ) {
        return itr->second.asString();
    }
    return "";
}

std::string Skill::getChargingEffectPos() {
    auto itr = _skill_data.find( "charging_effect_pos" );
    if( itr != _skill_data.end() ) {
        return itr->second.asString();
    }
    return "";
}

bool Skill::shouldContinue() {
    return _skill_data["multi_action"].asBool();
}