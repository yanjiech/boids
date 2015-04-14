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

using namespace cocos2d;

BlockNode::BlockNode() {
    
}

BlockNode::~BlockNode() {
    this->setEnabled( false );
}

BlockNode* BlockNode::create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BlockNode* ret = new BlockNode();
    if( ret && ret->init( grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BlockNode::init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !TargetNode::init() ) {
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
    
    const ValueMap& boundary_data = grid_properties.at( "boundary" ).asValueMap();
    _boundaries.loadFromValueMap( boundary_data );
    _boundaries.name = boundary_data.at( "name" ).asString();
    
    auto itr = obj_properties.find( "enabled" );
    if( itr != obj_properties.end() ) {
        this->setEnabled( itr->second.asBool() );
    }
    else {
        this->setEnabled( true );
    }
    
    _block_name = obj_properties.at( "name" ).asString();
    
    return true;
}

void BlockNode::setEnabled( bool b ) {
    _is_enabled = b;
    if( _is_enabled ) {
        _normal_sprite->setVisible( true );
        _destroyed_sprite->setVisible( false );
        for( auto pair : Terrain::getInstance()->getMeshes() ) {
            pair.second->removeCollidablePolygon( _boundaries.name );
        }
    }
    else {
        _normal_sprite->setVisible( false );
        _destroyed_sprite->setVisible( true );
        for( auto pair : Terrain::getInstance()->getMeshes() ) {
            pair.second->addCollidablePolygon( _boundaries );
        }
    }
}