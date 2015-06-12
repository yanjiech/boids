//
//  HintNode.cpp
//  Boids
//
//  Created by chenyanjie on 6/12/15.
//
//

#include "HintNode.h"

using namespace cocos2d;

HintNode::HintNode() {
    
    
}

HintNode::~HintNode() {
    
}

HintNode* HintNode::create( const std::string& name, cocos2d::Sprite* icon ) {
    HintNode* ret = new HintNode();
    if( ret && ret->init( name, icon ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool HintNode::init( const std::string& name, cocos2d::Sprite* icon ) {
    if( !Node::init() ) {
        return false;
    }
    this->setHintName( name );
    _icon = icon;
    this->setPosition( icon->getPosition() );
    icon->setPosition( Point::ZERO );
    
    this->addChild( _icon );
    
    return true;
}