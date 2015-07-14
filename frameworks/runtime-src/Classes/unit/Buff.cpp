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
#include "skill/SkillCache.h"

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
_buff_id( "" ),
_effect_resource( "" ),
_effect_scale( 1.0f )
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
    else if( buff_type == BUFF_TYPE_RECOVER ) {
        ret = RecoverBuff::create( owner, data );
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
    
    auto itr = _data.find( "effect_resource" );
    if( itr != _data.end() ) {
        _effect_resource = itr->second.asString();
    }
    
    itr = _data.find( "effect_scale" );
    if( itr != _data.end() ) {
        _effect_scale = itr->second.asFloat();
    }
    else {
        _effect_scale = 1.0f;
    }
    
    itr = _data.find( "effect_pos" );
    if( itr != _data.end() ) {
        _effect_pos = (eBuffEffectPos)itr->second.asInt();
    }
    else {
        _effect_pos = BuffEffectPosOrigin;
    }
    
    itr = _data.find( "effect_scale_x" );
    if( itr != _data.end() ) {
        _effect_scale_x = itr->second.asFloat();
    }
    else {
        _effect_scale_x = 1.0f;
    }
    
    itr = _data.find( "effect_scale_y" );
    if( itr != data.end() ) {
        _effect_scale_y = itr->second.asFloat();
    }
    else {
        _effect_scale_y = 1.0f;
    }
    
    itr = _data.find( "buff_group" );
    if( itr != data.end() ) {
        _buff_group = (eBuffGroup)itr->second.asInt();
    }
    else {
        _buff_group = BuffGroupDebuff;
    }
    
    _effect_color = Color3B::WHITE;
    
    return true;
}

void Buff::updateFrame( float delta ) {
    if( !_should_recycle && !this->isInfinite() ) {
        _elapse += delta;
    }
}

void Buff::begin() {
    if( !_effect_resource.empty() ) {
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( _effect_resource );
        skeleton->setScale( _effect_scale );
        skeleton->setScaleX( _effect_scale_x );
        skeleton->setScaleY( _effect_scale_y );
        skeleton->setColor( _effect_color );
        TimeLimitSpineComponent* effect = TimeLimitSpineComponent::create( _duration, skeleton, _buff_id + "_effect", true );
        effect->setAnimation( 0, "animation", true );
        switch ( _effect_pos ) {
            case BuffEffectPosOrigin:
                effect->setPosition( Point::ZERO );
                _owner->addUnitComponent( effect, effect->getName(), eComponentLayer::BelowObject );
                break;
            case BuffEffectPosBody:
                effect->setPosition( _owner->getLocalHitPos() );
                _owner->addUnitComponent( effect, effect->getName(), eComponentLayer::OverObject );
                break;
            case BuffEffectPosHead:
                effect->setPosition( _owner->getLocalHeadPos() );
                _owner->addUnitComponent( effect, effect->getName(), eComponentLayer::OverObject );
                break;
            default:
                break;
        }
    }
}

void Buff::end() {
    this->setShouldRecycle( true );
    _owner->removeUnitComponent( _buff_id + "_effect" );
}

UnitNode* Buff::getOwner() {
    return _owner;
}

void Buff::setOwner( UnitNode* owner ) {
    CC_SAFE_RETAIN( owner );
    CC_SAFE_RELEASE( _owner );
    _owner = owner;
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
    _owner->changeUnitState( eUnitState::UnderControl, true );
}

void StunBuff::end() {
    Buff::end();
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
    if( _elapse > _duration ) {
        this->end();
    }
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
}

//tag buff
TagBuff::TagBuff()
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
        _effect_resource = itr->second.asString();
        if( _effect_resource.find( "effects/" ) == std::string::npos ) {
            _effect_resource = "effects/" + _effect_resource;
        }
    }
    
    if( !_effect_resource.empty() ) {
        itr = _data.find( "color_red" );
        if( itr != _data.end() ) {
            _effect_color.r = itr->second.asByte();
        }
        
        itr = _data.find( "color_green" );
        if( itr != _data.end() ) {
            _effect_color.g = itr->second.asByte();
        }
        
        itr = _data.find( "color_blue" );
        if( itr != _data.end() ) {
            _effect_color.b = itr->second.asByte();
        }
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

}

void TagBuff::end() {
    Buff::end();
    _owner->removeUnitTag( _tag );
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
}

void TagProtectBuff::end() {
    Buff::end();
    _owner->removeUnitTag( _tag );
}

//recover buff
RecoverBuff::RecoverBuff() {
    
}

RecoverBuff::~RecoverBuff() {
    
}

RecoverBuff* RecoverBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    RecoverBuff* ret = new RecoverBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool RecoverBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    
    _hp = data.at( "hp" ).asFloat();
    _mp = data.at( "mp" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _recover_elapse = 0;
    
    return true;
}

void RecoverBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( _elapse > _duration ) {
        this->end();
    }
    else {
        _recover_elapse += delta;
        if( _recover_elapse > _interval ) {
            _recover_elapse = 0;
            if( _owner->isAlive() ) {
                ElementData* data = _owner->getTargetData();
                data->current_hp = clampf( data->current_hp + _hp, 0, data->hp );
                data->current_mp = clampf( data->current_mp + _mp, 0, data->mp );
                _owner->refreshHpAndMpBar();
            }
        }
    }
}

void RecoverBuff::begin() {
    Buff::begin();
}

void RecoverBuff::end() {
    Buff::end();
}

//bless buff
BlessBuff::BlessBuff() {
    
}

BlessBuff::~BlessBuff() {
    
}

BlessBuff* BlessBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    BlessBuff* ret = new BlessBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BlessBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !Buff::init( owner, data ) ) {
        return false;
    }
    return true;
}

void BlessBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( _elapse > _duration ) {
        this->end();
    }
}

void BlessBuff::begin() {
    Buff::begin();
    _owner->calculateTargetData();
}

void BlessBuff::end() {
    Buff::end();
    _owner->calculateTargetData();
}

//curse buff
CurseBuff::CurseBuff() {
    
}

CurseBuff::~CurseBuff() {
    
}

CurseBuff* CurseBuff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    CurseBuff* ret = new CurseBuff();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CurseBuff::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !BlessBuff::init( owner, data ) ) {
        return false;
    }
    
    return true;
}

void CurseBuff::updateFrame( float delta ) {
    Buff::updateFrame( delta );
    if( _elapse > _duration ) {
        this->end();
    }
}

void CurseBuff::begin() {
    Buff::begin();
    _owner->calculateTargetData();
}

void CurseBuff::end() {
    Buff::end();
    _owner->calculateTargetData();
}