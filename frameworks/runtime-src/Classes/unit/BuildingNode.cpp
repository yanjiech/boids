//
//  BuildingNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "BuildingNode.h"
#include "../manager/ResourceManager.h"

using namespace cocos2d;

BuildingNode::BuildingNode() {
    
}

BuildingNode::~BuildingNode() {
    
}

BuildingNode* BuildingNode::create(  const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BuildingNode* ret = new BuildingNode();
    if( ret && ret->init( grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BuildingNode::init(  const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !Node::init() ) {
        return false;
    }
    
    std::string source_name = grid_properties.at( "source" ).asString();
    _displayed_sprite = Sprite::createWithSpriteFrameName( source_name );
    _displayed_sprite->setPosition( Point( _displayed_sprite->getContentSize().width / 2, _displayed_sprite->getContentSize().height / 2 ) );
    this->addChild( _displayed_sprite );
    
    this->ignoreAnchorPointForPosition( false );
    this->setContentSize( _displayed_sprite->getContentSize() );
    this->setAnchorPoint( Point::ZERO );
    this->setPosition( Point( obj_properties.at( "x" ).asFloat(), obj_properties.at( "y" ).asFloat() ) );
    
    std::string vision_name = source_name.substr( 0, source_name.size() - 4 );
    const ValueMap& all_vision_config = ResourceManager::getInstance()->getVisionConfig();
    auto itr = all_vision_config.find( vision_name );
    if( itr != all_vision_config.end() ) {
        const ValueMap& vision_config = all_vision_config.at( vision_name ).asValueMap();
        this->setCenter( Point( vision_config.at( "x" ).asFloat(), vision_config.at( "y" ).asFloat() ) );
    }
    else {
        this->setCenter( Point( grid_properties.at( "width" ).asFloat() / 2, grid_properties.at( "height" ).asFloat() / 3.4641 ) );
    }
    
    if( grid_properties.at( "flipped_horizontally" ).asBool() ) {
        _displayed_sprite->setFlippedX( true );
    }
    else if( grid_properties.at( "flipped_vertically" ).asBool() ) {
        _displayed_sprite->setFlippedY( true );
    }
    else if( grid_properties.at( "flipped_diagonally" ).asBool() ) {
        _displayed_sprite->setFlippedX( true );
        _displayed_sprite->setFlippedY( true );
    }
    
    return true;
}