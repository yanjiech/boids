//
//  BlockNode.cpp
//  Boids
//
//  Created by chenyanjie on 4/3/15.
//
//

#include "BlockNode.h"
#include "UnitNode.h"
#include "../BoidsMath.h"
#include "../AI/Terrain.h"
#include "../ArmatureManager.h"

using namespace cocos2d;

BlockNode::BlockNode() {
    
}

BlockNode::~BlockNode() {
}

BlockNode* BlockNode::create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BlockNode* ret = nullptr;
    std::string block_type = obj_properties.at( "type" ).asString();
    if( block_type == "SpineBlockNode" ) {
        ret = SpineBlockNode::create( grid_properties, obj_properties );
    }
    else if( block_type == "SpriteBlockNode" ) {
        ret = SpriteBlockNode::create( grid_properties, obj_properties );
    }
    
    return ret;
}

bool BlockNode::init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !TargetNode::init( nullptr ) ) {
        return false;
    }
    
    const ValueMap& boundary_data = grid_properties.at( "boundary" ).asValueMap();
    _boundaries.loadFromValueMap( boundary_data );
    _boundaries.name = boundary_data.at( "name" ).asString();
    
    _block_name = obj_properties.at( "name" ).asString();
    
    return true;
}

void BlockNode::setEnabled( bool b ) {
    _is_enabled = b;
    if( _is_enabled ) {
        for( auto pair : Terrain::getInstance()->getMeshes() ) {
            pair.second->removeCollidablePolygon( _boundaries.name );
        }
    }
    else {
        for( auto pair : Terrain::getInstance()->getMeshes() ) {
            pair.second->addCollidablePolygon( _boundaries );
        }
    }
}

//sprite block node
SpriteBlockNode::SpriteBlockNode() {
    
}

SpriteBlockNode::~SpriteBlockNode() {
    
}

SpriteBlockNode* SpriteBlockNode::create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    SpriteBlockNode* ret = new SpriteBlockNode();
    if( ret && ret->init( grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SpriteBlockNode::init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !BlockNode::init( grid_properties, obj_properties ) ) {
        return false;
    }
    
    std::string frame_name = grid_properties.at( "source" ).asString();
    
    _normal_sprite = Sprite::createWithSpriteFrameName( frame_name );
    _normal_sprite->setPosition( Point( _normal_sprite->getContentSize().width / 2, _normal_sprite->getContentSize().height / 2 ) );
    this->addChild( _normal_sprite );
    
    frame_name = frame_name.substr( 0, frame_name.size() - 4 ) + "_destroyed.png";
    _destroyed_sprite = Sprite::createWithSpriteFrameName( frame_name );
    _destroyed_sprite->setPosition( _normal_sprite->getPosition() );
    this->addChild( _destroyed_sprite );
    
    this->ignoreAnchorPointForPosition( false );
    this->setContentSize( _normal_sprite->getContentSize() );
    this->setAnchorPoint( Point::ZERO );
    this->setPosition( Point( obj_properties.at( "x" ).asFloat(), obj_properties.at( "y" ).asFloat() ) );
    
    if( grid_properties.at( "flipped_horizontally" ).asBool() ) {
        _normal_sprite->setFlippedX( true );
        _destroyed_sprite->setFlippedX( true );
    }
    else if( grid_properties.at( "flipped_vertically" ).asBool() ) {
        _normal_sprite->setFlippedY( true );
        _destroyed_sprite->setFlippedX( true );
    }
    else if( grid_properties.at( "flipped_diagonally" ).asBool() ) {
        _normal_sprite->setFlippedX( true );
        _normal_sprite->setFlippedY( true );
        _destroyed_sprite->setFlippedX( true );
        _destroyed_sprite->setFlippedX( true );
    }
    
    auto itr = obj_properties.find( "enabled" );
    if( itr != obj_properties.end() ) {
        this->setEnabled( itr->second.asBool() );
    }
    else {
        this->setEnabled( true );
    }
    
    return true;
}

void SpriteBlockNode::setEnabled( bool b ) {
    BlockNode::setEnabled( b );
    if( _is_enabled ) {
        _normal_sprite->setVisible( true );
        _destroyed_sprite->setVisible( false );
    }
    else {
        _normal_sprite->setVisible( false );
        _destroyed_sprite->setVisible( true );
    }
}

//spine block node
SpineBlockNode::SpineBlockNode() {
    
}

SpineBlockNode::~SpineBlockNode() {
    
}

SpineBlockNode* SpineBlockNode::create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    SpineBlockNode* ret = new SpineBlockNode();
    if( ret && ret->init( grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SpineBlockNode::init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !BlockNode::init( grid_properties, obj_properties ) ) {
        return false;
    }
    
    std::string resource = "buidings/" + grid_properties.at( "source" ).asString();
    _skeleton = ArmatureManager::getInstance()->createArmature( resource );
    this->addChild( _skeleton );
    
    auto itr = obj_properties.find( "enabled" );
    if( itr != obj_properties.end() ) {
        this->setEnabled( itr->second.asBool() );
    }
    else {
        this->setEnabled( true );
    }
    
    return true;
}

void SpineBlockNode::setEnabled( bool b ) {
    BlockNode::setEnabled( b );
    
    if( _is_enabled ) {
        _skeleton->setAnimation( 0, "open", false );
    }
    else {
        _skeleton->setAnimation( 0, "close", false );
    }
}