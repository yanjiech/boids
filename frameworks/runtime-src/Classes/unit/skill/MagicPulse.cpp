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

using namespace cocos2d;

MagicPulse::MagicPulse() :
_target_unit( nullptr )
{
    
}

MagicPulse::~MagicPulse() {
    
}

MagicPulse* MagicPulse::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    MagicPulse* ret = new MagicPulse();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MagicPulse::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    
    
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
            else if( _owner->getPosition().distance( _target_unit->getPosition() ) > _range ) {
                this->end();
            }
            else {
                //adjust angle
                
                
                if( _damage_elapse >= _interval ) {
                    _damage_elapse = 0;
                    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_MAGIC_PULSE, _damage );
                    ValueMap result = calculator->calculateDamage( _owner->getUnitData(), _target_unit->getUnitData() );
                    _target_unit->takeDamage( result, _owner->getDeployId() );
                }
            }
        }
    }
}

void MagicPulse::begin() {
    SkillNode::begin();
    
    this->setTargetUnit( dynamic_cast<UnitNode*>( _owner->getChasingTarget() ) );
    if( _target_unit ) {
        std::string resource = "effects/";
        std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_MAGIC_PULSE, BulletNode::getNextBulletId() );
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        _component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
        _owner->getBattleLayer()->addToEffectLayer( _component, _owner->getEmitPos(), 0 );
        _component->setAnimation( 0, "animation", true );
        //adjust angle
    }
    else {
        this->end();
    }
}

void MagicPulse::end() {
    this->setTargetUnit( nullptr );
    _owner->endCast();
    SkillNode::end();
}

void MagicPulse::setTargetUnit( UnitNode* target_unit ) {
    CC_SAFE_RELEASE( _target_unit );
    _target_unit = target_unit;
    CC_SAFE_RELEASE( _target_unit );
}