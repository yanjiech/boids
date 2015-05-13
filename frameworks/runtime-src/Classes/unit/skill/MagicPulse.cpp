//
//  MagicPulse.cpp
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#include "MagicPulse.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../../BoidsMath.h"
#include "../../AI/Terrain.h"

using namespace cocos2d;

MagicPulse::MagicPulse() :
_target_unit( nullptr ),
_component( nullptr ),
_hit_component( nullptr )
{
    
}

MagicPulse::~MagicPulse() {
    
}

MagicPulse* MagicPulse::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    MagicPulse* ret = new MagicPulse();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MagicPulse::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = 0;
    _damage_elapse = 0;
    _range = data.at( "range" ).asFloat();;
    _length = data.at( "length" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    
    return true;
}

void MagicPulse::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        _damage_elapse += delta;
        
        if( _elapse >= _duration ) {
            this->end();
        }
        else {
            if( !_target_unit->isAlive() ) {
                this->end();
            }
            else if( !Math::isPositionInRange( _target_unit->getPosition(), _owner->getPosition(), _range + _target_unit->getUnitData()->collide ) ) {
                this->end();
            }
            else {
                //adjust angle
                this->adjustComponent();
                
                if( _damage_elapse >= _interval ) {
                    _damage_elapse = 0;
                    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_MAGIC_PULSE, _damage );
                    ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getUnitData(), _target_unit->getUnitData() );
                    _target_unit->takeDamage( result, _owner );
                }
            }
        }
    }
}

void MagicPulse::begin() {
    SkillNode::begin();
    
    UnitNode* target_unit = dynamic_cast<UnitNode*>( _owner->getChasingTarget() );
    if( target_unit == nullptr || Math::isPositionInRange( target_unit->getPosition(), _owner->getPosition(), _range + target_unit->getUnitData()->collide ) || Terrain::getInstance()->isBlocked( _owner->getPosition(), target_unit->getPosition() ) ) {
        Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _owner->getPosition(), _range );
        int size = (int)candidates.size();
        if( size > 0 ) {
            int rand = Utils::randomNumber( size ) - 1;
            target_unit = candidates.at( rand );
        }
    }
    if( target_unit ) {
        this->setTargetUnit( target_unit );
        std::string resource = "effects/women_wizard_skill_1/pulse";
        std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_MAGIC_PULSE, BulletNode::getNextBulletId() );
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        _component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
        _owner->getBattleLayer()->addToEffectLayer( _component, Point::ZERO, 0 );
        _component->setAnimation( 0, "animation", true );
        
        //hit effect
        resource = "effects/women_wizard_skill_1/hit";
        name = Utils::stringFormat( "%s_%d", SKILL_NAME_MAGIC_PULSE, BulletNode::getNextBulletId() );
        spine::SkeletonAnimation* hit_skeleton = ArmatureManager::getInstance()->createArmature( resource );
        _hit_component = TimeLimitSpineComponent::create( _duration, hit_skeleton, name, true );
        _target_unit->addUnitComponent( _hit_component, _hit_component->getName(), eComponentLayer::OverObject );
        _hit_component->setPosition( _target_unit->getLocalHitPos() );
        _hit_component->setAnimation( 0, "animation", true );
        
        this->adjustComponent();
    }
    else {
        this->end();
    }
}

void MagicPulse::end() {
    if( _component ) {
        _component->setDuration( 0 );
    }
    if( _hit_component ) {
        _hit_component->setDuration( 0 );
    }
    this->setTargetUnit( nullptr );
    _owner->endCast();
    SkillNode::end();
}

void MagicPulse::setTargetUnit( UnitNode* target_unit ) {
    CC_SAFE_RELEASE( _target_unit );
    _target_unit = target_unit;
    CC_SAFE_RETAIN( _target_unit );
}

void MagicPulse::adjustComponent() {
    //adjust angle
    Point emit_pos = _owner->getEmitPos();
    Point hit_pos = _target_unit->getHitPos();
    Point dir = hit_pos - emit_pos;
    float angle = dir.getAngle();
    float distance = hit_pos.distance( emit_pos );
    _component->setScale( distance / _length );
    _component->setRotation( -angle * 180.0f / M_PI );
    _component->setPosition( emit_pos );
}