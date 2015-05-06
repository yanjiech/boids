//
//  BlockNode.cpp
//  Boids
//
//  Created by chenyanjie on 4/3/15.
//
//

#include "BlockNode.h"
#include "../BoidsMath.h"
#include "../AI/Terrain.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

BlockNode::BlockNode() :
_range_sprite( nullptr )
{
    
}

BlockNode::~BlockNode() {
}

BlockNode* BlockNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BlockNode* ret = nullptr;
    std::string block_type = obj_properties.at( "type" ).asString();
    if( block_type == "GroupSpineBlockNode" ) {
        ret = GroupSpineBlockNode::create( battle_layer, grid_properties, obj_properties );
    }
    else if( block_type == "SpriteBlockNode" ) {
        ret = SpriteBlockNode::create( battle_layer, grid_properties, obj_properties );
    }
    
    return ret;
}

bool BlockNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !TargetNode::init( nullptr ) ) {
        return false;
    }
    
    _battle_layer = battle_layer;
     _block_name = obj_properties.at( "name" ).asString();
    
    const ValueMap& boundary_data = grid_properties.at( "boundary" ).asValueMap();
    _boundaries.loadFromValueMap( boundary_data );
    _boundaries.name = boundary_data.at( "name" ).asString();
    _center = _boundaries.center;
    
    auto itr = obj_properties.find( "need_repair" );
    if( itr != obj_properties.end() ) {
        _need_repair = itr->second.asBool();
    }
    else {
        _need_repair = false;
    }
    
    if( _need_repair ) {
        _range = obj_properties.at( "range" ).asFloat();
        _need_time = obj_properties.at( "need_time" ).asFloat();
        std::string tag_str = obj_properties.at( "need_tag" ).asString();
        Utils::split( tag_str, _need_tag, ',' );
        this->setEnabled( false );
        
        Sprite* sprite = Sprite::createWithSpriteFrameName( "block_range.png" );
        this->setRangeSprite( sprite );
        sprite->setVisible( false );
        
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
    }
    else {
        itr = obj_properties.find( "enabled" );
        if( itr != obj_properties.end() ) {
            this->setEnabled( itr->second.asBool() );
        }
        else {
            this->setEnabled( true );
        }
    }
    _elapse = 0;
    
    _progress_bar = ProgressBar::create( Color4F::WHITE, Color4F::WHITE, Size( 290.0f, 10.0f ) );
    _progress_bar->setBackgroundOpacity( 127 );
    this->addChild( _progress_bar, 10 );
    
    return true;
}

void BlockNode::updateFrame( float delta ) {
    if( _need_repair ) {
        Vector<UnitNode*> candidates = _battle_layer->getAliveUnitsByCondition( eTargetCamp::Player, _need_tag, _center, _range );
        int count = (int)candidates.size();
        if( count == 0 ) {
            _elapse = 0;
            _progress_bar->setPercentage( 0 );
            _progress_bar->setVisible( false );
            if( _range_sprite ) {
                _range_sprite->setVisible( false );
            }
        }
        else {
            _progress_bar->setVisible( true );
            if( _range_sprite ) {
                _range_sprite->setVisible( true );
            }
            UnitNode* unit = candidates.at( 0 );
            
            _elapse += delta;
            if( _elapse >= _need_time ) {
                _need_repair = false;
                this->setEnabled( true );
                this->updateEnabled();
                _progress_bar->setVisible( false );
                for( auto str : _need_tag ) {
                    unit->removeItem( str );
                }
                if( _range_sprite ) {
                    _range_sprite->setVisible( false );
                }
            }
            else {
                _progress_bar->setPercentage( 100.0f * _elapse / _need_time );
            }
        }
    }
}

void BlockNode::updateEnabled() {
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
    ValueMap update_data;
    update_data["trigger_type"] = Value( "vision_change" );
    update_data["source_value"] = Value( _block_name );
    update_data["vision_state"] = Value( _is_enabled ? VISION_STATE_ENABLED : VISION_STATE_DISABLED );
    MapLogic* map_logic = _battle_layer->getMapLogic();
    if( map_logic ) {
        map_logic->onVisionChanged( update_data );
    }
}

void BlockNode::setRangeSprite( cocos2d::Sprite* image ) {
    if( _range_sprite ) {
        _range_sprite->removeFromParentAndCleanup( true );
    }
    _range_sprite = image;
    if( _range_sprite ) {
        _battle_layer->addToBelowObjectLayer( _range_sprite, _center, 100 );
    }
}

//sprite block node
SpriteBlockNode::SpriteBlockNode() {
    
}

SpriteBlockNode::~SpriteBlockNode() {
    
}

SpriteBlockNode* SpriteBlockNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    SpriteBlockNode* ret = new SpriteBlockNode();
    if( ret && ret->init( battle_layer, grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SpriteBlockNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !BlockNode::init( battle_layer, grid_properties, obj_properties ) ) {
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
    
    this->updateEnabled();
    
    _progress_bar->setPercentage( 0 );
    _progress_bar->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height + 50.0f ) );
    _progress_bar->setVisible( false );
    
    return true;
}

void SpriteBlockNode::updateEnabled() {
    BlockNode::updateEnabled();
    if( _is_enabled ) {
        _normal_sprite->setVisible( true );
        _destroyed_sprite->setVisible( false );
    }
    else {
        _normal_sprite->setVisible( false );
        _destroyed_sprite->setVisible( true );
    }
}

//group spine block component
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
    if( !Node::init() ) {
        return false;
    }
    
    std::string source = grid_properties.at( "source" ).asString();
    size_t p = source.find( ".png" );
    if( p != std::string::npos ) {
        source = source.substr( 0, p );
    }
    std::string resource = "blocks/" + source;
    _skeleton = ArmatureManager::getInstance()->createArmature( resource );
    _skeleton->setCompleteListener( CC_CALLBACK_1( SpineBlockNode::onSkeletonAnimationCompleted, this ) );
    Size size = _skeleton->getBoundingBox().size;
    this->addChild( _skeleton );
    this->setContentSize( size );
    this->setPosition( Point( obj_properties.at( "x" ).asFloat() + size.width / 2, obj_properties.at( "y" ).asFloat() + size.width / 2 ) );
    
    return true;
}

void SpineBlockNode::setAnimation( int track, const std::string& name, bool loop ) {
    if( _skeleton ) {
        _skeleton->setAnimation( track, name, loop );
    }
}

void SpineBlockNode::onSkeletonAnimationCompleted( int track_index ) {
    spTrackEntry* entry = _skeleton->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    if( animation_name == "Appear" ) {
        _skeleton->addAnimation( 0, "Idle", true );
    }
}

//group spine block node
GroupSpineBlockNode::GroupSpineBlockNode() {
    
}

GroupSpineBlockNode::~GroupSpineBlockNode() {
    
}

GroupSpineBlockNode* GroupSpineBlockNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    GroupSpineBlockNode* ret = new GroupSpineBlockNode();
    if( ret && ret->init( battle_layer, grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GroupSpineBlockNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !BlockNode::init( battle_layer, grid_properties, obj_properties ) ) {
        return false;
    }
    this->setEnabled( false );
    
    this->appendSpineNode( grid_properties, obj_properties );
    
    SpineBlockNode* block_node = _blocks.at( 0 );

    this->ignoreAnchorPointForPosition( false );
    this->setAnchorPoint( Point::ZERO );
    this->setPosition( Point( obj_properties.at( "x" ).asFloat(), obj_properties.at( "y" ).asFloat() ) );
    
    _progress_bar->setPercentage( 0 );
    _progress_bar->setPosition( Point( 0, block_node->getContentSize().height + 50.0f ) );
    _progress_bar->setVisible( false );
    this->setPosition( _center );
    
    this->updateEnabled();
    
    return true;
}

void GroupSpineBlockNode::addBlockNode( SpineBlockNode* block_node ) {
    _blocks.pushBack( block_node );
    block_node->setPosition( block_node->getPosition() - _center );
    this->addChild( block_node );
}

void GroupSpineBlockNode::appendSpineNode( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    SpineBlockNode* block_node = SpineBlockNode::create( grid_properties, obj_properties );
    this->addBlockNode( block_node );
    
    auto itr = obj_properties.find( "need_repair" );
    if( itr != obj_properties.end() ) {
        _need_repair = itr->second.asBool();
        if( _need_repair ) {
            _range = obj_properties.at( "range" ).asFloat();
            _need_time = obj_properties.at( "need_time" ).asFloat();
            std::string tag_str = obj_properties.at( "need_tag" ).asString();
            Utils::split( tag_str, _need_tag, ',' );
            this->setEnabled( false );
            
            Sprite* sprite = Sprite::createWithSpriteFrameName( "block_range.png" );
            this->setRangeSprite( sprite );
            sprite->setVisible( false );
            
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
        }
    }
    
    this->updateEnabled();
}

void GroupSpineBlockNode::updateEnabled() {
    BlockNode::updateEnabled();
    if( _is_enabled ) {
        for( auto block : _blocks ) {
            block->setAnimation( 0, "Disappear", false );
        }
    }
    else {
        for( auto block : _blocks ) {
            block->setAnimation( 0, "Appear", false );
        }
    }
}
