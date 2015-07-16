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
    BattleLayer* battle_layer = _owner->getBattleLayer();
    
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
    
    //invoke monster
    cocos2d::Vector<UnitNode*> deploy_units;
    for( int i = 0; i < _count; ++i ) {
        ValueMap unit_data;
        unit_data["name"] = Value( "white_ghost" );
        unit_data["level"] = Value( _level );
        unit_data["show_hp"] = Value( false );
        unit_data["is_boss"] = Value( false );
        unit_data["hold_position"] = Value( false );
        unit_data["unit_camp"] = Value( UnitNode::getStringByCamp( _owner->getTargetCamp() ) );
        
        UnitNode* unit = UnitNode::create( _owner->getBattleLayer(), unit_data );
        deploy_units.pushBack( unit );
        
        SkillBehavior* skill_behavior = SkillBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_SKILL, skill_behavior );
        AttackBehavior* attack_behavior = AttackBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
        MoveBehavior* move_behavior = MoveBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_MOVE, move_behavior );
        IdleBehavior* idle_behavior = IdleBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_IDLE, idle_behavior );
    }
    Rect deploy_rect = Rect( _owner->getPosition().x - _radius, _owner->getPosition().y - _radius, _radius * 2, _radius * 2 );
    battle_layer->deployUnits( deploy_units, deploy_rect, _owner->getSightGroup() );
}

void EvilInvoke::end() {
    
}