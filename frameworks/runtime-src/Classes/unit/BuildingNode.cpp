//
//  BuildingNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "BuildingNode.h"
#include "../manager/ResourceManager.h"
#include "../AI/Terrain.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

BuildingNode::BuildingNode() :
_range_sprite( nullptr )
{
    
}

BuildingNode::~BuildingNode() {
    
}

BuildingNode* BuildingNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BuildingNode* ret = nullptr;
    std::string type = obj_properties.at( "type" ).asString();
    if( type == "BuffBuildingNode" ) {
        ret = BuffBuildingNode::create( battle_layer, grid_properties, obj_properties );
    }
    
    return ret;
}

bool BuildingNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !TargetNode::init( battle_layer ) ) {
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
    
    auto itr = grid_properties.find( "boundary" );
    if( itr != grid_properties.end() ) {
        const ValueMap& boundary_data = itr->second.asValueMap();
        _boundaries.loadFromValueMap( boundary_data );
        _boundaries.name = boundary_data.at( "name" ).asString();
        this->setCenter( _boundaries.center );
    }
    else {
        this->setCenter( this->getPosition() );
    }
    
    _building_name = obj_properties.at( "name" ).asString();
    this->setCollidable( true );
    this->setEnabled( true );
    
    return true;
}

void BuildingNode::setCollidable( bool b ) {
    _is_collidable = b;
    if( !_is_collidable ) {
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

void BuildingNode::setRangeSprite( cocos2d::Sprite* sprite ) {
    if( _range_sprite ) {
        _range_sprite->removeFromParentAndCleanup( true );
    }
    _range_sprite = sprite;
    if( _range_sprite ) {
        _battle_layer->addToBelowObjectLayer( _range_sprite, _center, 100 );
    }
}

//buff building node
BuffBuildingNode::BuffBuildingNode() {
    
}

BuffBuildingNode::~BuffBuildingNode() {
    
}

BuffBuildingNode* BuffBuildingNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BuffBuildingNode* ret = new BuffBuildingNode();
    if( ret && ret->init( battle_layer, grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BuffBuildingNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !BuildingNode::init( battle_layer, grid_properties, obj_properties ) ) {
        return false;
    }
    
    _range = obj_properties.at( "range" ).asFloat();
    _elapse = 0;
    
    _buff_data["duration"] = obj_properties.at( "buff_duration" );
    _buff_data["buff_type"] = obj_properties.at( "buff_type" );
    _buff_data["buff_name"] = obj_properties.at( "buff_name" );
    _buff_data["buff_param"] = obj_properties.at( "buff_param" );
    
    auto itr = obj_properties.find( "stay_duration" );
    if( itr != obj_properties.end() ) {
        _duration = itr->second.asFloat();
    }
    else {
        _duration = 0;
    }
    
    itr = obj_properties.find( "effect_resource" );
    if( itr != obj_properties.end() ) {
        _buff_data["effect_resource"] = itr->second;
    }
    
    itr = obj_properties.find( "effect_scale" );
    if( itr != obj_properties.end() ) {
        _buff_data["effect_scale"] = itr->second;
    }
    
    Sprite* sprite = Sprite::createWithSpriteFrameName( "block_range.png" );
    this->setRangeSprite( sprite );
    
    Color3B color = Color3B::WHITE;
    
    itr = obj_properties.find( "color_red" );
    if( itr != obj_properties.end() ) {
        color.r = itr->second.asByte();
    }
    
    itr = obj_properties.find( "color_green" );
    if( itr != obj_properties.end() ) {
        color.g = itr->second.asByte();
    }
    
    itr = obj_properties.find( "color_blue" );
    if( itr != obj_properties.end() ) {
        color.b = itr->second.asByte();
    }
    
    sprite->setColor( color );
    
    sprite->setScale( _range / 200.0f );
    
    _progress_bar = ProgressBar::create( Color4F::WHITE, Color4F::WHITE, Size( 290.0f, 10.0f ) );
    _progress_bar->setBackgroundOpacity( 127 );
    this->addChild( _progress_bar, 10 );
    _progress_bar->setPosition( this->getContentSize().width / 2, this->getContentSize().height + 50.0f );
    _progress_bar->setVisible( false );
    
    return true;
}

void BuffBuildingNode::updateFrame( float delta ) {
    if( _is_enabled ) {
        Vector<UnitNode*> candidates = _battle_layer->getAliveAllyInRange( eTargetCamp::Player, this->getCenter(), _range );
        if( candidates.size() == 0 ) {
            _elapse = 0;
            _progress_bar->setVisible( false );
        }
        else {
            _progress_bar->setVisible( true );
            _elapse += delta;
            if( _elapse > _duration ) {
                Vector<UnitNode*> units = _battle_layer->getAliveUnitsByCamp( eTargetCamp::Player );
                for( auto itr = units.begin(); itr != units.end(); ++itr ) {
                    UnitNode* unit = *itr;
                    Buff* buff = Buff::create( unit, _buff_data );
                    unit->addBuff( buff->getBuffId(), buff, true );
                    buff->begin();
                }
                _progress_bar->setVisible( false );
            }
            else {
                _progress_bar->setPercentage( 100.0f * _elapse / _duration );
            }
        }
    }
}