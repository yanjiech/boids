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
    
    _is_active = true;
    
    return true;
}

void BattleHintNode::updateHintPos( class BattleLayer* battle_layer, const cocos2d::Point& pos ) {
    if( _is_active ) {
        Rect screen_rect = battle_layer->getScreenRect();
        if( battle_layer->getLeaderUnit() != nullptr ) {
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
    }
}

void BattleHintNode::setActive( bool b ) {
    _is_active = b;
}

bool BattleHintNode::isActive() {
    return _is_active;
}

//hint node
HintNode::HintNode() :
_hint_icon( nullptr ),
_effect( nullptr )
{
}

HintNode::~HintNode() {
    if( _hint_icon ) {
        _hint_icon->removeFromParent();
    }
}

HintNode* HintNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& obj_properties, const cocos2d::ValueMap& grid_properties ) {
    HintNode* ret = new HintNode();
    if( ret && ret->init( battle_layer, obj_properties, grid_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool HintNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& obj_properties, const cocos2d::ValueMap& grid_properties ) {
    if( !Node::init() ) {
        return false;
    }
    _battle_layer = battle_layer;
    
    this->setHintName( obj_properties.at( "name" ).asString() );
    auto itr = obj_properties.find( "hint_type" );
    if( itr != obj_properties.end() ) {
        this->setHintType( itr->second.asString() );
    }
    else {
        this->setHintType( "" );
    }
    
    std::string icon_resource = grid_properties.at( "source" ).asString();
    _icon = Sprite::createWithSpriteFrameName( icon_resource );
    this->setPosition( _icon->getPosition() );
    _icon->setPosition( Point::ZERO );
    
    this->addChild( _icon, 1 );
    
    if( _type == BATTLE_HINT_DESTINATION ) {
        _hint_icon = BattleHintNode::create( BATTLE_HINT_DESTINATION );
        _battle_layer->addToFloatLayer( _hint_icon, Point::ZERO, 0 );
        
        _effect = ArmatureManager::getInstance()->createArmature( "effects/common/final_destination" );
        this->addChild( _effect, 0 );
        _effect->setAnimation( 0, "animation", true );
    }
    
    if( grid_properties.at( "flipped_horizontally" ).asBool() ) {
        _icon->setFlippedX( true );
    }
    if( grid_properties.at( "flipped_vertically" ).asBool() ) {
        _icon->setFlippedY( true );
    }
    if( grid_properties.at( "flipped_diagonally" ).asBool() ) {
        _icon->setFlippedX( true );
        _icon->setFlippedY( true );
    }
    
    this->setPosition( Point( obj_properties.at( "x" ).asFloat(), obj_properties.at( "y" ).asFloat() ) );
    
    return true;
}

void HintNode::updateFrame( float delta ) {
    if( _hint_icon ) {
        _hint_icon->updateHintPos( _battle_layer , this->getPosition() );
    }
}

void HintNode::setVisible( bool b ) {
    Node::setVisible( b );
    if( _hint_icon ) {
        _hint_icon->setActive( b );
        _hint_icon->setVisible( b );
    }
    if( _effect ) {
        _effect->setVisible( b );
    }
}


