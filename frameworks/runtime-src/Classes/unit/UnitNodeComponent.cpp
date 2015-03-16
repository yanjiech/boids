//
//  UnitNodeComponent.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#include "UnitNodeComponent.h"

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
    
    skeleton->setEndListener( CC_CALLBACK_1( UnitNodeSpineComponent::onSkeletonAnimationEnded, this ) );
    return true;
}

bool UnitNodeSpineComponent::setAnimation( int track_index, const std::string& name, bool loop ) {
    spine::SkeletonAnimation* skeleton = dynamic_cast<spine::SkeletonAnimation*>( _node );
    if( skeleton ) {
        return skeleton->setAnimation( track_index, name, loop ) != nullptr;
    }
    return false;
}

void UnitNodeSpineComponent::onSkeletonAnimationEnded( int track_index ) {
    if( _auto_recycle ) {
        _should_recycle = true;
    }
}