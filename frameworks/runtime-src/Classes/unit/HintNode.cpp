//
//  HintNode.cpp
//  Boids
//
//  Created by chenyanjie on 6/12/15.
//
//

#include "HintNode.h"
#include "../constant/BoidsConstant.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

//battle hint node
BattleHintNode::BattleHintNode() :
_nd_arrow( nullptr ) {
    
}

BattleHintNode::~BattleHintNode() {
    
}

BattleHintNode* BattleHintNode::create( const std::string& hint_name ) {
    BattleHintNode* ret = new BattleHintNode();
    if( ret && ret->init( hint_name ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BattleHintNode::init( const std::string& hint_name ) {
    if( !Node::init() ) {
        return false;
    }
    
    Sprite* icon = nullptr;
    if( hint_name == BATTLE_HINT_BOSS ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_boss.png" );
    }
    else if( hint_name == BATTLE_HINT_DESTINATION ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_escort.png" );
    }
    else if( hint_name == BATTLE_HINT_GUARD ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_guard.png" );
    }
    else if( hint_name == BATTLE_HINT_HOSTAGE || hint_name == BATTLE_HINT_VILLIGER ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_sos.png" );
    }
    else if( hint_name == BATTLE_HINT_ITEM ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_wupin.png" );
    }
    else if( hint_name == BATTLE_HINT_MASTER ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_guai.png" );
    }
    else if( hint_name == BATTLE_HINT_VICE_BOSS ) {
        icon = Sprite::createWithSpriteFrameName( "ui_battle_hint_boss_2.png" );
    }
    this->addChild( icon, 1 );
    
    Sprite* arrow = Sprite::createWithSpriteFrameName( "ui_battle_hint_jiantou.png" );
    arrow->setPosition( Point( 0, icon->getContentSize().height / 2 - 10.0f ) );
    
    _nd_arrow = Node::create();
    _nd_arrow->addChild( arrow );
    this->addChild( _nd_arrow );
    
    return true;
}

void BattleHintNode::setHintRotation( float angle ) {
    if( _nd_arrow ) {
        _nd_arrow->setRotation( angle + 90.0f );
    }
}

void BattleHintNode::updateHintPos( class BattleLayer* battle_layer, const cocos2d::Point& pos ) {
    
}

//hint node
HintNode::HintNode() :
_hint_icon( nullptr )
{
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

void HintNode::updateFrame( float delta ) {

}


