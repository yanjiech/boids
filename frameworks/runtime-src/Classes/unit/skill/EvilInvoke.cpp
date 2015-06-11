//
//  EvilInvoke.cpp
//  Boids
//
//  Created by chenyanjie on 6/11/15.
//
//

#include "EvilInvoke.h"
#include "../../scene/BattleLayer.h"

using namespace cocos2d;

EvilInvoke::EvilInvoke() {
    
}

EvilInvoke::~EvilInvoke() {
    
}

EvilInvoke* EvilInvoke::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    EvilInvoke* ret = new EvilInvoke();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EvilInvoke::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    _count = data.at( "count" ).asInt();
    _level = data.at( "level" ).asInt();
    _radius = data.at( "radius" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    
    return true;
}

void EvilInvoke::updateFrame( float delta ) {
    
}

void EvilInvoke::begin() {
    std::string resource = "effects/evil_wizard_skill_1/down";
    std::string name = std::string( SKILL_NAME_EVIL_INVOKE ) + "_down";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    TimeLimitSpineComponent* component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
    _owner->addUnitComponent( component, name, eComponentLayer::BelowObject );
    component->setPosition( Point::ZERO );
    component->setAnimation( 0, "animation", true );
    
    resource = "effects/evil_wizard_skill_1/up";
    name = std::string( SKILL_NAME_EVIL_INVOKE ) + "_up";
    skeleton = ArmatureManager::getInstance()->createArmature( resource );
    component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
    _owner->addUnitComponent( component, name, eComponentLayer::OverObject );
    component->setPosition( Point::ZERO );
    component->setAnimation( 0, "animation", true );
}

void EvilInvoke::end() {
    
}