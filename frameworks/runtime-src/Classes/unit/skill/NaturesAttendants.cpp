//
//  NaturesAttendants.cpp
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#include "NaturesAttendants.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../UnitNodeComponent.h"

using namespace cocos2d;

NaturesAttendants::NaturesAttendants() {
    
}

NaturesAttendants::~NaturesAttendants() {
    
}

NaturesAttendants* NaturesAttendants::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    NaturesAttendants* ret = new NaturesAttendants();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool NaturesAttendants::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _heal = data.at( "hp" ).asValueVector().at( level - 1 ).asFloat();
    _range = data.at( "range" ).asFloat();
    _count = data.at( "count" ).asValueVector().at( level - 1 ).asInt();
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    _interval = data.at( "interval" ).asFloat();
    _heal_elapse = 0;
    _init_speed = data.at( "init_speed" ).asFloat();
    _accel = data.at( "accel" ).asFloat();
    
    return true;
}

void NaturesAttendants::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        _heal_elapse += delta;
        
        if( _elapse > _duration ) {
            this->end();
        }
        else {
            if( _heal_elapse >= _interval ) {
                _heal_elapse = 0;
                Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveAllyInRange( _owner->getTargetCamp(), _owner->getPosition(), _range );
                for( auto unit : candidates ) {
                    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_NATURESATTENDANTS, _heal );
                    ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getTargetData(), unit->getTargetData() );
                    result["cri"] = Value( false );
                    unit->takeHeal( result, _owner->getDeployId() );
                }
            }
        }
    }
}

void NaturesAttendants::begin() {
    SkillNode::begin();
    std::string resource = "effects/enchantress_skill_1/charge";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_NATURESATTENDANTS, BulletNode::getNextBulletId() );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
    component->setPosition( Point::ZERO );
    _owner->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
    component->setAnimation( 0, "animation", false );
    
    for( int i = 0; i < _count; ++i ) {
        resource = "effects/enchantress_skill_1/spirit";
        skeleton = ArmatureManager::getInstance()->createArmature( resource );
        name = Utils::stringFormat( "%s_%d", SKILL_NAME_NATURESATTENDANTS, BulletNode::getNextBulletId() );
        ValueMap spirit_data;
        spirit_data["duration"] = Value( _duration );
        spirit_data["init_speed"] = Value( _init_speed );
        spirit_data["accelerate"] = Value( _accel );
        spirit_data["range"] = Value( _range );
        spirit_data["interval"] = Value( _interval );
        TimeLimitWanderSpineComponent* spirit_component = TimeLimitWanderSpineComponent::create( spirit_data, _owner, skeleton, name, true );
        spirit_component->setAnimation( 0, "animation", true );
        _owner->getBattleLayer()->addToEffectLayer( spirit_component, _owner->getHitPos(), 0 );
        _effects.pushBack( spirit_component );
    }
}

void NaturesAttendants::end() {
    SkillNode::end();
    for( auto effect : _effects ) {
        effect->setShouldRecycle( true );
    }
}