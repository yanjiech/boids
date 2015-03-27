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

Buff* Buff::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    Buff* ret = nullptr;
    std::string buff_type = data.at( "buff_type" ).asString();
    if( buff_type == BUFF_TYPE_SHIELD ) {
        ret = ShieldBuff::create( owner, data );
    }
    else if( buff_type == BUFF_TYPE_STUN ) {
        
    }
    else if( buff_type == BUFF_TYPE_BURN ) {
        
    }
    else if( buff_type == BUFF_TYPE_PIERCE ) {
        
    }
    else if( buff_type == BUFF_TYPE_POISON ) {
        
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