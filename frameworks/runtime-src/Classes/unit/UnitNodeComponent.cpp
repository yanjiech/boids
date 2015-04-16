//
//  UnitNodeComponent.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#include "UnitNodeComponent.h"
#include "../scene/BattleLayer.h"
#include "../data/DamageCalculate.h"
#include "../Utils.h"
#include "../BoidsMath.h"

using namespace cocos2d;

UnitNodeComponent::UnitNodeComponent() {
    
}

UnitNodeComponent::~UnitNodeComponent() {
    
}

UnitNodeComponent* UnitNodeComponent::create( cocos2d::Node* node, const std::string& name, bool auto_recycle ) {
    UnitNodeComponent* ret = new UnitNodeComponent();
    if( ret && ret->init( node, name, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitNodeComponent::init( cocos2d::Node* node, const std::string& name, bool auto_recycle ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->setName( name );
    _auto_recycle = auto_recycle;
    _should_recycle = false;
    
    _node = node;
    this->addChild( _node );

    return true;
}

void UnitNodeComponent::updateFrame( float delta ) {
    
}

TimeLimitComponent::TimeLimitComponent() {
    
}

TimeLimitComponent::~TimeLimitComponent() {
    
}

TimeLimitComponent* TimeLimitComponent::create( float duration, cocos2d::Node* node, const std::string& name, bool auto_recycle ) {
    TimeLimitComponent* ret = new TimeLimitComponent();
    if( ret && ret->init( duration, node, name, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TimeLimitComponent::init( float duration, cocos2d::Node* node, const std::string& name, bool auto_recycle ) {
    if( !UnitNodeComponent::init( node, name, auto_recycle ) ) {
        return false;
    }
    
    _duration = duration;
    _elapse = 0;
    
    return true;
}

void TimeLimitComponent::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->setShouldRecycle( true );
        }
    }
}

UnitNodeSpineComponent::UnitNodeSpineComponent() {
    
}

UnitNodeSpineComponent::~UnitNodeSpineComponent() {
    
}

UnitNodeSpineComponent* UnitNodeSpineComponent::create( spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    UnitNodeSpineComponent* ret = new UnitNodeSpineComponent();
    if( ret && ret->init( skeleton, name, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitNodeSpineComponent::init( spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    if( !UnitNodeComponent::init( skeleton, name, auto_recycle ) ) {
        return false;
    }
    
    skeleton->setCompleteListener( CC_CALLBACK_1( UnitNodeSpineComponent::onSkeletonAnimationCompleted, this ) );
    return true;
}

bool UnitNodeSpineComponent::setAnimation( int track_index, const std::string& name, bool loop ) {
    spine::SkeletonAnimation* skeleton = dynamic_cast<spine::SkeletonAnimation*>( _node );
    if( skeleton ) {
        return skeleton->setAnimation( track_index, name, loop ) != nullptr;
    }
    return false;
}

void UnitNodeSpineComponent::onSkeletonAnimationCompleted( int track_index ) {
    if( _auto_recycle ) {
        _should_recycle = true;
    }
}

UnitNodeFollowSpineComponent::UnitNodeFollowSpineComponent() :
_source_unit( nullptr )
{
    
}

UnitNodeFollowSpineComponent::~UnitNodeFollowSpineComponent() {
    CC_SAFE_RELEASE( _source_unit );
}

UnitNodeFollowSpineComponent* UnitNodeFollowSpineComponent::create( UnitNode* source_unit, const std::string& bone_name, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    UnitNodeFollowSpineComponent* ret = new UnitNodeFollowSpineComponent();
    if( ret && ret->init( source_unit, bone_name, skeleton, name, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitNodeFollowSpineComponent::init( UnitNode* source_unit, const std::string& bone_name, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    if( !UnitNodeSpineComponent::init( skeleton, name, auto_recycle ) ) {
        return false;
    }
    this->setSourceUnit( source_unit );
    _bone_name = bone_name;
    return true;
}

void UnitNodeFollowSpineComponent::updateFrame( float delta ) {
    if( _bone_name != "" ) {
        Point new_pos = _source_unit->getLocalBonePos( _bone_name );
        this->setPosition( new_pos );
    }
}

void UnitNodeFollowSpineComponent::setSourceUnit( UnitNode* source_unit ) {
    CC_SAFE_RELEASE( _source_unit );
    _source_unit = source_unit;
    CC_SAFE_RETAIN( _source_unit );
}

TimeLimitSpineComponent::TimeLimitSpineComponent() {
    
}

TimeLimitSpineComponent::~TimeLimitSpineComponent() {
    
}

TimeLimitSpineComponent* TimeLimitSpineComponent::create( float duration, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    TimeLimitSpineComponent* ret = new TimeLimitSpineComponent();
    if( ret && ret->init( duration, skeleton, name, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TimeLimitSpineComponent::init( float duration, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    if( !UnitNodeComponent::init( skeleton, name, auto_recycle ) ) {
        return false;
    }
    
    _duration = duration;
    _elapse = 0;
    
    return true;
}

void TimeLimitSpineComponent::updateFrame( float delta ) {
    _elapse += delta;
    if( _elapse > _duration ) {
        this->setShouldRecycle( true );
    }
}

bool TimeLimitSpineComponent::setAnimation( int track_index, const std::string& name, bool loop ) {
    spine::SkeletonAnimation* skeleton = dynamic_cast<spine::SkeletonAnimation*>( _node );
    if( skeleton ) {
        return skeleton->setAnimation( track_index, name, loop ) != nullptr;
    }
    return false;
}

TimeLimitWanderSpineComponent::TimeLimitWanderSpineComponent() :
_source_unit( nullptr )
{
    
}

TimeLimitWanderSpineComponent::~TimeLimitWanderSpineComponent() {
    CC_SAFE_RELEASE( _source_unit );
}

TimeLimitWanderSpineComponent* TimeLimitWanderSpineComponent::create( const cocos2d::ValueMap& data, UnitNode* source_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    TimeLimitWanderSpineComponent* ret = new TimeLimitWanderSpineComponent();
    if( ret && ret->init( data, source_unit, skeleton, name, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TimeLimitWanderSpineComponent::init( const cocos2d::ValueMap& data, UnitNode* source_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle ) {
    float duration = data.at( "duration" ).asFloat();
    if( !TimeLimitSpineComponent::init( duration, skeleton, name, auto_recycle ) ) {
        return false;
    }
    
    _init_speed = data.at( "init_speed" ).asFloat();
    _accelarate = data.at( "accelerate" ).asFloat();
    _range = data.at( "range" ).asFloat();
    _accel_change_interval = data.at( "interval" ).asFloat();
    _accel_change_elapse = _accel_change_interval;
    this->setSourceUnit( source_unit );
    float angle = Utils::randomFloat() * M_PI * 2.0f;
    _speed = Point( cosf( angle ) * _init_speed, sinf( angle ) * _init_speed );
    _gravity_point = Point::ZERO;
    
    return true;
}

void TimeLimitWanderSpineComponent::updateFrame( float delta ) {
    TimeLimitSpineComponent::updateFrame( delta );
    if( !_should_recycle ) {
        Point center = _source_unit->getHitPos();
         _accel_change_elapse += delta;
        if( _accel_change_elapse > _accel_change_interval ) {
            _accel_change_interval = 0;
            _gravity_point = Utils::randomPositionInRange( center, _range );
        }
        
        if( !Math::isPositionInRange( this->getPosition(), center, _range ) ) {
            _speed = Point::ZERO;
        }
        
        float d = center.distance( this->getPosition() );
        float ratio = 0.5f + d * 1.5 / _range;
        Point acc = _gravity_point - this->getPosition();
        acc.normalize();
        acc *= ( _accelarate * ratio );
        _speed += ( acc * delta );
        Point new_pos = this->getPosition() + _speed * delta;
        this->setPosition( new_pos );
    }
}

void TimeLimitWanderSpineComponent::setSourceUnit( UnitNode* source_unit ) {
    CC_SAFE_RELEASE( _source_unit );
    _source_unit = source_unit;
    CC_SAFE_RETAIN( source_unit );
}

UnitNodeSpineDamageComponent::UnitNodeSpineDamageComponent() :
_source_unit( nullptr ),
_target_unit( nullptr ),
_damage_calculator( nullptr )
{
    
}

UnitNodeSpineDamageComponent::~UnitNodeSpineDamageComponent() {
    CC_SAFE_RELEASE( _source_unit );
    CC_SAFE_RELEASE( _target_unit );
    CC_SAFE_RELEASE( _damage_calculator );
}

UnitNodeSpineDamageComponent* UnitNodeSpineDamageComponent::create( UnitNode* source_unit, UnitNode* target_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle, DamageCalculate* calculator ) {
    UnitNodeSpineDamageComponent* ret = new UnitNodeSpineDamageComponent();
    if( ret && ret->init( source_unit, target_unit, skeleton, name, auto_recycle, calculator ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitNodeSpineDamageComponent::init( UnitNode* source_unit, UnitNode* target_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle, DamageCalculate* calculator ) {
    if( !UnitNodeSpineComponent::init( skeleton, name, auto_recycle ) ) {
        return false;
    }
    this->setSourceUnit( source_unit );
    this->setTargetUnit( target_unit );
    this->setDamageCalculator( calculator );
    skeleton->setEventListener( CC_CALLBACK_2( UnitNodeSpineDamageComponent::onSkeletonAnimationEvent, this ) );
    
    return true;
}

void UnitNodeSpineDamageComponent::onSkeletonAnimationEvent( int track_index, spEvent* event ) {
    ValueMap result = _damage_calculator->calculateDamage( _source_unit->getTargetData(), _target_unit->getTargetData() );
    if( !_target_unit->isDying() ) {
        _target_unit->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), _source_unit->getDeployId() );
    }
}

void UnitNodeSpineDamageComponent::setSourceUnit( UnitNode* source_unit ) {
    CC_SAFE_RELEASE( _source_unit );
    _source_unit = source_unit;
    CC_SAFE_RETAIN( _source_unit );
}

void UnitNodeSpineDamageComponent::setTargetUnit( UnitNode* target_unit ) {
    CC_SAFE_RELEASE( _target_unit );
    _target_unit = target_unit;
    CC_SAFE_RETAIN( _target_unit );
}

void UnitNodeSpineDamageComponent::setDamageCalculator( DamageCalculate* calculator ) {
    CC_SAFE_RELEASE( _damage_calculator );
    _damage_calculator = calculator;
    CC_SAFE_RETAIN( _damage_calculator );
}

UnitNodeFadeoutComponent::UnitNodeFadeoutComponent() {
    
}

UnitNodeFadeoutComponent::~UnitNodeFadeoutComponent() {
    
}

UnitNodeFadeoutComponent* UnitNodeFadeoutComponent::create( cocos2d::Node* node, const std::string& name, float duration, int fadeto, bool auto_recycle ) {
    UnitNodeFadeoutComponent* ret = new UnitNodeFadeoutComponent();
    if( ret && ret->init( node, name, duration, fadeto, auto_recycle ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitNodeFadeoutComponent::init( cocos2d::Node* node, const std::string& name, float duration, int fadeto, bool auto_recycle ) {
    if( !UnitNodeComponent::init( node, name, auto_recycle ) ) {
        return false;
    }
    
    FadeTo* fade_action = FadeTo::create( duration, fadeto );
    CallFunc* callback = CallFunc::create( CC_CALLBACK_0( UnitNodeFadeoutComponent::onFadeoutEnd, this ) );
    Sequence* seq = Sequence::create( fade_action, callback, nullptr );
    node->runAction( seq );
    
    return true;
}

void UnitNodeFadeoutComponent::onFadeoutEnd() {
    this->setShouldRecycle( true );
}