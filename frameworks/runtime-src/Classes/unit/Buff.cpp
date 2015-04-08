//
//  Buff.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#include "Buff.h"
#include "UnitNode.h"
#include "UnitNodeComponent.h"
#include "../Utils.h"

using namespace cocos2d;

int Buff::_global_buff_id = 0;

int Buff::getNextBuffId() {
    if( ++_global_buff_id == 0x00ffffff ) {
        _global_buff_id = 0;
    }
    return _global_buff_id;
}

Buff::Buff() :
_owner( nullptr ),
_buff_id( "" )
{
    
}

Buff::~Buff() {
    
}

Buff* Buff::clone() {
    return Buff::create( _owner, _data );
}

Buff* Buff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    Buff* ret = nullptr;
    std::string buff_type = data.at( "buff_type" ).asString();
    if( buff_type == BUFF_TYPE_SHIELD ) {
        ret = ShieldBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_STUN ) {
        
    }
    else if( buff_type == BUFF_TYPE_BURN ) {
        ret = BurnBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_PIERCE ) {
        ret = PierceBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_POISON ) {
        ret = PoisonBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_SLOW ) {
        ret = SlowBuff::create( owner, data );
    }
    return ret;
}

bool Buff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    _should_recycle = false;
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    _buff_id = Utils::stringFormat( "%s_%d", data.at( "buff_name" ).asString().c_str(), Buff::getNextBuffId() );
    this->setOwner( owner );
    this->setBuffData( data );
    return true;
}

void Buff::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
    }
}

void Buff::begin() {
    
}

void Buff::end() {
    this->setShouldRecycle( true );
}

UnitNode* Buff::getOwner() {
    return _owner;
}

void Buff::setOwner( UnitNode* owner ) {
    CC_SAFE_RELEASE( _owner );
    _owner = owner;
    CC_SAFE_RETAIN( _owner );
}

PoisonBuff::PoisonBuff() {
    
}

PoisonBuff::~PoisonBuff() {
    
}

PoisonBuff* PoisonBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    PoisonBuff* ret = new PoisonBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PoisonBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    return true;
}

void PoisonBuff::updateFrame( float delta ) {
    
}

void PoisonBuff::begin() {
    Buff::begin();
}

void PoisonBuff::end() {
    Buff::end();
}

//slow buff
SlowBuff::SlowBuff() {
    
}
SlowBuff::~SlowBuff() {
    
}

SlowBuff* SlowBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    SlowBuff* ret = new SlowBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SlowBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    _slow_percent = data.at( "slow_percent" ).asFloat();
    
    return true;
}

void SlowBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    
    if( _elapse > _duration ) {
        this->end();
    }
}

void SlowBuff::begin() {
    Buff::begin();
    
    _owner->getUnitData()->move_speed *= _slow_percent;
}

void SlowBuff::end() {
    Buff::end();
    
    _owner->getUnitData()->move_speed /= _slow_percent;
}

ShieldBuff::ShieldBuff() {
    
}

ShieldBuff::~ShieldBuff() {
    
}
    
ShieldBuff* ShieldBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    ShieldBuff* ret = new ShieldBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ShieldBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    _buff_type = BUFF_TYPE_SHIELD;
    _absorb = _data.at( "absorb" ).asFloat();
    return true;
}
    
void ShieldBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( _elapse > _duration ) {
        this->end();
    }
}

void ShieldBuff::begin() {
    std::string resource = _data.at( "effect_resource" ).asString();
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    auto itr = _data.find( "effect_scale" );
    if( itr != _data.end() ) {
        skeleton->setScale( itr->second.asFloat() );
    }
    TimeLimitSpineComponent* shield = TimeLimitSpineComponent::create( _duration, skeleton, _buff_id + "_effect", true );
    shield->setPosition( Point::ZERO );
    shield->setAnimation( 0, "animation", true );
    _owner->addUnitComponent( shield, shield->getName(), eComponentLayer::OverObject );
    Buff::begin();
}

void ShieldBuff::end() {
    _owner->removeUnitComponent( _buff_id + "_effect" );
    Buff::end();
}

int ShieldBuff::absorbDamage( float damage ) {
    float ret = 0;
    if( _absorb < damage ) {
        _absorb = 0;
        ret = damage - _absorb;
        this->end();
    }
    return ret;
}

PierceBuff::PierceBuff() {
    
}

PierceBuff::~PierceBuff() {
    
}

PierceBuff* PierceBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    PierceBuff* ret = new PierceBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PierceBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    return true;
}

void PierceBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( _elapse > _duration ) {
        this->end();
    }
}

void PierceBuff::begin() {
    Buff::begin();

    std::string resource = "effects/dracula_skill_1/dark_pierce/down";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    TimeLimitSpineComponent* pierce_bottom = TimeLimitSpineComponent::create( _duration, skeleton, _buff_id + "_b", true );
    _owner->addUnitComponent( pierce_bottom, pierce_bottom->getName(), eComponentLayer::OverObject );
    pierce_bottom->setAnimation( 0, "animation", false );
    pierce_bottom->setPosition( _owner->getLocalHitPos() + Point( 0, 30.0f ) );
    
    resource = "effects/dracula_skill_1/dark_pierce/up";
    skeleton = ArmatureManager::getInstance()->createArmature( resource );
    TimeLimitSpineComponent* pierce_top = TimeLimitSpineComponent::create( _duration, skeleton, _buff_id + "_f", true );
    _owner->addUnitComponent( pierce_top, pierce_top->getName(), eComponentLayer::BelowObject );
    pierce_top->setAnimation( 0, "animation", false );
    pierce_top->setPosition( _owner->getLocalHitPos() + Point( 0, 30.0f ) );
    
    resource = "effects/dracula_skill_1/bat";
    skeleton = ArmatureManager::getInstance()->createArmature( resource );
    TimeLimitSpineComponent* bat_component = TimeLimitSpineComponent::create( _duration, skeleton, _buff_id + "_bat", true );
    _owner->addUnitComponent( bat_component, bat_component->getName(), eComponentLayer::OverObject );
    bat_component->setAnimation( 0, "animation", true );
    bat_component->setPosition( _owner->getLocalHeadPos() );
    
    resource = "effects/bullets/blood_hit";
    skeleton = ArmatureManager::getInstance()->createArmature( resource );
    TimeLimitSpineComponent* blood_component = TimeLimitSpineComponent::create( _duration, skeleton, _buff_id + "_blood", true );
    _owner->addUnitComponent( blood_component, blood_component->getName(), eComponentLayer::OverObject );
    blood_component->setAnimation( 0, "animation", true );
    blood_component->setPosition( _owner->getLocalHitPos() );
    
    _owner->riseup( 0.2f, 30.0f );
    _owner->changeUnitState( eUnitState::UnderControl, true );
}

void PierceBuff::end() {
    Buff::end();
    
    _owner->falldown( 0, 30.0f );
    _owner->removeUnitComponent( _buff_id + "_b" );
    _owner->removeUnitComponent( _buff_id + "_f" );
    
    _owner->changeUnitState( eUnitState::Idle, true );
    
}