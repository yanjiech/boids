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
    this->ignoreAnchorPointForPosition( false );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    this->setContentSize( icon->getContentSize() );
    icon->setPosition( Point( icon->getContentSize().width / 2, icon->getContentSize().height / 2 ) );
    this->addChild( icon, 1 );
    
    Sprite* arrow = Sprite::createWithSpriteFrameName( "ui_battle_hint_jiantou.png" );
    arrow->setPosition( Point( 0, -icon->getContentSize().height / 2 - 10.0f ) );
    
    _nd_arrow = Node::create();
    _nd_arrow->setAnchorPoint( Point( 0.5f, 0.5f ) );
    _nd_arrow->ignoreAnchorPointForPosition( false );
    _nd_arrow->setContentSize( Size( 0, 0 ) );
    _nd_arrow->addChild( arrow );
    _nd_arrow->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _nd_arrow, 2 );
    
    return true;
}

void BattleHintNode::updateHintPos( class BattleLayer* battle_layer, const cocos2d::Point& pos ) {
    Rect screen_rect = battle_layer->getScreenRect();
    Point leader_pos = battle_layer->getLeaderUnit()->getPosition();
    Point screen_center = Point( screen_rect.origin.x + screen_rect.size.width / 2, screen_rect.origin.y + screen_rect.size.height / 2 );
    float half_width = screen_rect.size.width / 2;
    float half_height = screen_rect.size.height / 2;
    
    if( screen_rect.containsPoint( pos ) ) {
        this->setVisible( false );
    }
    else {
        this->setVisible( true );
        Point insect_point;
        Point dir = Point( pos.x - screen_center.x, pos.y - screen_center.y );
        float angle = atan2f( dir.y, dir.x );
        float f_angle = atan2f( screen_rect.size.height / 2, half_width );
        if( angle <= f_angle && angle >= -f_angle ) {
            insect_point = Point( screen_center.x + half_width, screen_center.y + half_width * tanf( angle ) );
        }
        else if( angle <= M_PI - f_angle && angle > f_angle ) {
            insect_point = Point( screen_center.x + half_height / tanf( angle ), screen_center.y + half_height );
        }
        else if( angle < -f_angle && angle >= -M_PI + f_angle ) {
            insect_point = Point( screen_center.x - half_height / tanf( angle ), screen_center.y - half_height );
        }
        else {
            insect_point = Point( screen_center.x - half_width, screen_center.y - half_width * tanf( angle ) );
        }
        
        dir.normalize();
        insect_point = insect_point + -dir * this->getContentSize().width;
        
        this->setPosition( insect_point );
        _nd_arrow->setRotation( -angle * 180.0f / M_PI - 90.0f );
    }
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


