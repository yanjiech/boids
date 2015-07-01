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
    else if( type == "ChestNode" ) {
        ret = ChestNode::create( battle_layer, grid_properties, obj_properties );
    }
    
    return ret;
}

bool BuildingNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !TargetNode::init( battle_layer ) ) {
        return false;
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
        _buff_data["color_red"] = itr->second;
    }
    
    itr = obj_properties.find( "color_green" );
    if( itr != obj_properties.end() ) {
        color.g = itr->second.asByte();
        _buff_data["color_green"] = itr->second;
    }
    
    itr = obj_properties.find( "color_blue" );
    if( itr != obj_properties.end() ) {
        color.b = itr->second.asByte();
        _buff_data["color_blue"] = itr->second;
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

//chest node
ChestNode::ChestNode() :
_open_effect( nullptr )
{
    
}

ChestNode::~ChestNode() {
    
}

ChestNode* ChestNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    ChestNode* ret = new ChestNode();
    if( ret && ret->init( battle_layer, grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ChestNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !BuildingNode::init( battle_layer, grid_properties, obj_properties ) ) {
        return false;
    }
    
    _skeleton = ArmatureManager::getInstance()->createArmature( "buildings/chest" );
    this->addChild( _skeleton );
    _skeleton->setAnimation( 0, "Appear", true );
    
    this->setPosition( _center );
    
    int i = 1;
    do {
        std::string key = Utils::stringFormat( "item_%d", i );
        auto itr = obj_properties.find( key );
        if( itr != obj_properties.end() ) {
            int item_id = itr->second.asInt();
            key = Utils::stringFormat( "item_rate_%d", i );
            float rate = obj_properties.at( key ).asFloat();
            
            _drop_items.insert( std::make_pair( item_id, Value( rate ) ) );
            i++;
        }
        else {
            break;
        }
    }while( true );
    
    _range = obj_properties.at( "range" ).asFloat();
    
    _progress_bar = ProgressBar::create( Color4F::WHITE, Color4F::WHITE, Size( 290.0f, 10.0f ) );
    _progress_bar->setBackgroundOpacity( 127 );
    this->addChild( _progress_bar, 10 );
    _progress_bar->setPosition( 0, _skeleton->getBoundingBox().size.height + 10.0f );
    _progress_bar->setVisible( false );
    
    return true;
}

void ChestNode::updateFrame( float delta ) {
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
                _progress_bar->setVisible( false );
                
                this->openChest();
            }
            else {
                _progress_bar->setPercentage( 100.0f * _elapse / _duration );
            }
        }
    }
}

void ChestNode::openChest() {
    //play effect
    _skeleton->setAnimation( 0, "animation", false );
    
    if( _open_effect == nullptr ) {
        _open_effect = ArmatureManager::getInstance()->createArmature( "buildings/chest_opened" );
        this->addChild( _open_effect, 2 );
    }
    _open_effect->setVisible( true );
    _open_effect->setCompleteListener( CC_CALLBACK_1( ChestNode::onOpenEffectAnimationCompleted, this ) );
    _open_effect->setAnimation( 0, "Appear", false );
    
    //drop items
    for( auto pair : _drop_items ) {
        int item_id = pair.first;
        float rate = pair.second.asFloat();
        if( Utils::randomFloat() <= rate ) {
            ValueMap item_data;
            item_data["item_id"] = Value( item_id );
            item_data["count"] = Value( 1 );
            DropItem* item = DropItem::create( item_data );
            _battle_layer->dropItem( item, this->getPosition(), eBattleSubLayer::ObjectLayer );
            
            Point drop_pos = Utils::randomPositionInRange( this->getPosition(), _range, 2 * _range );
            Rect region = Rect( drop_pos.x - 25.0f, drop_pos.y - 25.0f, 50.0f, 50.0 );
            Point desired_pos = _battle_layer->getAvailablePosition( 50.0f, region );
            if( desired_pos.equals( Point::ZERO ) ) {
                desired_pos = this->getPosition() + Point( _range, _range );
            }
            
            item->dropTo( desired_pos );
        }
    }
    this->setEnabled( false );
}

void ChestNode::onOpenEffectAnimationCompleted( int track_index ) {
    _open_effect->setVisible( false );
}