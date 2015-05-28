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
#include "BulletNode.h"

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
    else if( buff_type == BUFF_TYPE_UNDEAD ) {
        ret = UndeadBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_STUN ) {
        ret = StunBuff::create( owner, data );
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
    else if( buff_type == BUFF_TYPE_ATTRIBUTE ) {
        ret = AttributeBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_TAG  ) {
        ret = TagBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_TAG_PROTECT ) {
        ret = TagProtectBuff::create( owner, data );
    }
    return ret;
}

bool Buff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    _should_recycle = false;
    _buff_type = data.at( "buff_type" ).asString();
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    _buff_id = Utils::stringFormat( "%s_%d", data.at( "buff_name" ).asString().c_str(), Buff::getNextBuffId() );
    this->setOwner( owner );
    this->setBuffData( data );
    return true;
}

void Buff::updateFrame( float delta ) {
    if( !_should_recycle && !this->isInfinite() ) {
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

//attribute buff
AttributeBuff::AttributeBuff() :
_unit_data( nullptr )
{
    
}

AttributeBuff::~AttributeBuff() {
    CC_SAFE_RELEASE( _unit_data );
}

AttributeBuff* AttributeBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    AttributeBuff* ret = new AttributeBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool AttributeBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    ElementData* unit_data = ElementData::create( ValueMap() );
    const ValueMap& attributes = data.at( "attributes" ).asValueMap();
    for( auto pair : attributes ) {
        unit_data->setAttribute( pair.first, pair.second.asString() );
    }
    this->setElementData( unit_data );
    
    return true;
}

void AttributeBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( !this->isInfinite() && _elapse > _duration ) {
        this->end();
    }
}

void AttributeBuff::begin() {
    _owner->getTargetData()->add( _unit_data );
    Buff::begin();
}

void AttributeBuff::end() {
    _owner->getTargetData()->sub( _unit_data );
    Buff::end();
}

ElementData* AttributeBuff::getElementData() {
    return _unit_data;
}

void AttributeBuff::setElementData( ElementData* data ) {
    CC_SAFE_RELEASE( _unit_data );
    _unit_data = data;
    CC_SAFE_RETAIN( _unit_data );
}

//stun buff
StunBuff::StunBuff() {
    
}

StunBuff::~StunBuff() {
    
}

StunBuff* StunBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    StunBuff* ret = new StunBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool StunBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    return true;
}

void StunBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( !this->isInfinite() && _elapse > _duration ) {
        this->end();
    }
}

void StunBuff::begin() {
    Buff::begin();
    std::string resource = "effects/stunned";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    std::string name = Utils::stringFormat( "stunned" );
    _component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
    _owner->addUnitComponent( _component, _component->getName(), eComponentLayer::OverObject );
    _component->setPosition( Point( _owner->getContentSize().width / 2, _owner->getContentSize().height ) );
    _component->setAnimation( 0, "animation", true );
    _owner->changeUnitState( eUnitState::UnderControl, true );
}

void StunBuff::end() {
    Buff::end();
    _component->setDuration( 0 );
    _owner->changeUnitState( eUnitState::Idle, true );
}


//poison buff
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
    
    if( !this->isInfinite() && _elapse > _duration ) {
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

//shield buff
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
    if( !this->isInfinite() && _elapse > _duration ) {
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

float ShieldBuff::filterDamage( float damage, TargetNode* atker ) {
    float ret = 0;
    if( _absorb < damage ) {
        _absorb = 0;
        ret = damage - _absorb;
        this->end();
    }
    return ret;
}

//undead buff
UndeadBuff::UndeadBuff() {
    
}

UndeadBuff::~UndeadBuff() {
    
}

UndeadBuff* UndeadBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    UndeadBuff* ret = new UndeadBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UndeadBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    return true;
}

void UndeadBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
}

void UndeadBuff::begin() {
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

void UndeadBuff::end() {
    _owner->removeUnitComponent( _buff_id + "_effect" );
    Buff::end();
}

float UndeadBuff::filterDamage( float damage, TargetNode* atker ) {
    float ret = damage;
    float c_hp = _owner->getTargetData()->current_hp;
    if( damage >= c_hp ) {
        ret = c_hp - 1.0f;
    }
    return ret;
}


//pierce buff
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
    if( !this->isInfinite() && _elapse > _duration ) {
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

//tag buff
TagBuff::TagBuff() :
_component( nullptr )
{
    
}

TagBuff::~TagBuff() {
    
}

TagBuff* TagBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    TagBuff* ret = new TagBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TagBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    _tag = data.at( "buff_param" ).asString();
   
    
    auto itr = data.find( "effect_resource" );
    if( itr != data.end() ) {
        float duration = data.at( "duration" ).asFloat();
        std::string effect_resource = itr->second.asString();
        std::string resource = "effects/" + effect_resource;
        Color3B color = Color3B::WHITE;
        
        auto sitr = _data.find( "color_red" );
        if( sitr != _data.end() ) {
            color.r = sitr->second.asByte();
        }
        else {
            color.r = 0;
        }
        
        sitr = _data.find( "color_green" );
        if( sitr != _data.end() ) {
            color.g = sitr->second.asByte();
        }
        else {
            color.g = 0;
        }
        
        sitr = _data.find( "color_blue" );
        if( sitr != _data.end() ) {
            color.b = sitr->second.asByte();
        }
        else {
            color.b = 0;
        }
        
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        skeleton->setColor( color );
        _component = TimeLimitSpineComponent::create( duration, skeleton, _buff_id, true );
        _component->setPosition( _owner->getLocalHitPos() );
        _owner->addUnitComponent( _component, _component->getName(), eComponentLayer::OverObject );
        _component->setAnimation( 0, "animation", true );
    }
    
    return true;
}

void TagBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( !this->isInfinite() && _elapse > _duration ) {
        this->end();
    }
}

void TagBuff::begin() {
    Buff::begin();
    _owner->addUnitTag( _tag );
    if( _component ) {
        _component->setAnimation( 0, "animation", true );
    }
}

void TagBuff::end() {
    Buff::end();
    _owner->removeUnitTag( _tag );
    if( _component ) {
        _component->setDuration( 0 );
    }
}

//tag protect buff
TagProtectBuff::TagProtectBuff()
{
    
}

TagProtectBuff::~TagProtectBuff() {
    
}

TagProtectBuff* TagProtectBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    TagProtectBuff* ret = new TagProtectBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TagProtectBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !TagBuff::init( owner, data ) ) {
        return false;
    }
    
    _tag = data.at( "buff_param" ).asString();
    
    auto itr = data.find( "effect_name" );
    if( itr != data.end() ) {
        std::string effect_name = itr->second.asString();
        
        std::string resource = "effects/" + effect_name;
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        _component = TimeLimitSpineComponent::create( INT_MAX, skeleton, _buff_id, true );
        _owner->addUnitComponent( _component, _component->getName(), eComponentLayer::OverObject );
    }
    
    return true;
}

void TagProtectBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( !this->isInfinite() && _elapse > _duration ) {
        this->end();
    }
}

float TagProtectBuff::filterDamage( float damage, TargetNode* atker ) {
    std::string tag_break = _tag;
    if( atker && atker->hasUnitTag( tag_break ) ) {
        return damage;
    }
    else {
        return 1;
    }
}

void TagProtectBuff::begin() {
    Buff::begin();
    _owner->addUnitTag( _tag );
    if( _component ) {
        _component->setAnimation( 0, "animation", true );
    }
}

void TagProtectBuff::end() {
    Buff::end();
    _owner->removeUnitTag( _tag );
    if( _component ) {
        _component->setDuration( 0 );
    }
}