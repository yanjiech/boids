//
//  BattleLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "BattleLayer.h"
#include <iostream>
#include "../Utils.h"
#include "../AI/Terrain.h"
#include "../BoidsMath.h"
#include "../constant/BoidsConstant.h"
#include "../unit/skill/SkillCache.h"
#include "../manager/AudioManager.h"
#include "../unit/skill/SkillCache.h"

#define FOG_TILE_SIZE 32
#define FOG_TILE_SIZE_WIDTH 110
#define FOG_TILE_SIZE_HEIGHT 128

using namespace cocos2d;

eBattleState BattleLayer::getBattleStateFromString( const std::string& str ) {
    if( str == GAME_STATE_WIN ) {
        return eBattleState::BattleWin;
    }
    else if( str == GAME_STATE_LOSE ) {
        return eBattleState::BattleLose;
    }
    else if( str == GAME_STATE_PREPARE ) {
        return eBattleState::BattlePrepare;
    }
    else if( str == GAME_STATE_RUNNING ) {
        return eBattleState::BattleRunning;
    }
    else if( str == GAME_STATE_PAUSED ) {
        return eBattleState::BattlePaused;
    }
    
    return eBattleState::UnknownBattleState;
}

BattleLayer::BattleLayer() :
_map_data( nullptr ),
_map_logic( nullptr ),
_should_show_fog( false ),
_fog_tile_visible_array( nullptr )
{
}

BattleLayer::~BattleLayer() {
    CC_SAFE_RELEASE( _map_data );
    CC_SAFE_RELEASE( _map_logic );
    CC_SAFE_DELETE_ARRAY( _fog_tile_visible_array );
}

BattleLayer* BattleLayer::create( MapData* map_data, const std::string& level_id, bool is_pvp ) {
    BattleLayer* ret = new BattleLayer();
    if( ret && ret->init( map_data, level_id, is_pvp ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BattleLayer::init( MapData* map_data, const std::string& level_id, bool is_pvp ) {
    if( !Layer::init() ) {
        return false;
    }
    do {
        this->setLevelId( level_id );
        this->setMapData( map_data );
        
        std::set<float> all_collide_radius;
        all_collide_radius.insert( 30.0f );
//        all_collide_radius.insert( 40.0f );
        all_collide_radius.insert( 50.0f );
        Terrain::getInstance()->parseMap( _map_data->getMapData(), all_collide_radius );
        
        const ValueMap& meta_json = _map_data->getMetaJson();
        auto itr = meta_json.find( "options" );
        if( itr != meta_json.end() ) {
            const ValueMap& options = itr->second.asValueMap();
            itr = options.find( "should_show_fog" );
            if( itr != options.end() ) {
                this->setShouldShowFog( itr->second.asBool() );
            }
        }
        
        cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
        cocos2d::Size size = Director::getInstance()->getVisibleSize();
    
        _tmx_map = map_data->generateTiledMapWithFlags( 1 );
        _tmx_map->setPosition( origin );
        this->addChild( _tmx_map, 0, eBattleSubLayer::MapLayer );
    
        auto grass_layer = _tmx_map->getLayer( "grass" );
        if( grass_layer ) {
            grass_layer->setLocalZOrder( eBattleSubLayer::GrassLayer );
        }
        
        _max_map_position = origin;
        _min_map_position = Point( origin.x + size.width - _tmx_map->getContentSize().width, origin.y + size.height - _tmx_map->getContentSize().height );
        
        _on_ground_layer = Layer::create();
        _tmx_map->addChild( _on_ground_layer, eBattleSubLayer::OnGroundLayer, eBattleSubLayer::OnGroundLayer );
        
        _below_object_layer = Layer::create();
        _tmx_map->addChild( _below_object_layer, eBattleSubLayer::BelowObjectLayer, eBattleSubLayer::BelowObjectLayer );
        
        _object_layer = Layer::create();
        _tmx_map->addChild( _object_layer, eBattleSubLayer::ObjectLayer, eBattleSubLayer::ObjectLayer );
        
        _over_object_layer = Layer::create();
        _tmx_map->addChild( _over_object_layer, eBattleSubLayer::OverObjectLayer, eBattleSubLayer::OverObjectLayer );
        
        _effect_layer = Layer::create();
        _tmx_map->addChild( _effect_layer, eBattleSubLayer::EffectLayer, eBattleSubLayer::EffectLayer );
        
        _float_layer = UIBattleFloatLayer::create( this );
        this->addChild( _float_layer, eBattleSubLayer::FloatLayer, eBattleSubLayer::FloatLayer );
        
        _toast_layer = Layer::create();
        this->addChild( _toast_layer, eBattleSubLayer::ToastLayer, eBattleSubLayer::ToastLayer );
        
        _skill_ui_layer = UIBattleLayer::create( this );
        this->addChild( _skill_ui_layer, eBattleSubLayer::BattleUILayer, eBattleSubLayer::BattleUILayer );
        
        _control_layer = UIControlLayer::create();
        this->addChild( _control_layer, eBattleSubLayer::ControlLayer, eBattleSubLayer::ControlLayer );
        
        _story_layer = UIStoryLayer::create( CC_CALLBACK_1( BattleLayer::endStory, this ) );
        this->addChild( _story_layer, eBattleSubLayer::BattleStoryLayer, eBattleSubLayer::BattleStoryLayer );
        _story_layer->setVisible( false );
        
        _battle_menu_layer = UIBattleMenuLayer::create( this );
        this->addChild( _battle_menu_layer, eBattleSubLayer::BattleMenuLayer, eBattleSubLayer::BattleMenuLayer );
        
        _draw_node = DrawNode::create();
        _tmx_map->addChild( _draw_node, 100000 );
        
        this->setup();
        
        this->startBattle();
        
        this->schedule( CC_CALLBACK_1( BattleLayer::updateFrame, this ), "battle_update"  );
        
        if( _should_show_fog ) {
            _fog_node = Node::create();
            this->addChild( _fog_node, eBattleSubLayer::FogLayer );
            
            _fog_x_tile_count = int( ceil( 1920 / FOG_TILE_SIZE ) ) + 1;
            _fog_y_tile_count = int( ceil( 1080 / FOG_TILE_SIZE ) ) + 1;
            
            if( FOG_TILE_SIZE == 32 ) {
                _fog_texture = Director::getInstance()->getTextureCache()->addImage( "ui/ui_fog_mask_32.png" );
            }
            else if( FOG_TILE_SIZE == 64 ) {
                _fog_texture = Director::getInstance()->getTextureCache()->addImage( "ui/ui_fog_mask_64.png" );
            }
            
            
            for( int j = 0; j < _fog_y_tile_count; j++ ) {
                for( int i = 0; i < _fog_x_tile_count; i++ ) {
                    Sprite* sp = Sprite::createWithTexture( _fog_texture, Rect( 0, 0, FOG_TILE_SIZE, FOG_TILE_SIZE ) );
                    sp->setAnchorPoint( Point::ZERO );
                    sp->setPosition( Point( i * FOG_TILE_SIZE, j * FOG_TILE_SIZE ) );
                    
                    _fog_node->addChild( sp );
                    sp->setOpacity( 200 );
                    sp->setTag( j * _fog_x_tile_count + i );
                }
            }
            
            CC_SAFE_DELETE_ARRAY( _fog_tile_visible_array );
            _fog_tile_visible_array = new unsigned char[_fog_x_tile_count * _fog_y_tile_count];
            memset( _fog_tile_visible_array, 0, _fog_x_tile_count * _fog_y_tile_count );
        }
        
        return true;
    }while( 0 );
    return false;
}

void BattleLayer::setup() {
    this->reset();
    this->parseMapObjects();
    MapLogic* new_map_logic = MapLogic::create( this );
    this->setMapLogic( new_map_logic );
    _map_logic->onMapInit();
    _game_time = 0;
    _battle_menu_layer->loadTasks();
}

void BattleLayer::reset() {
    _state = eBattleState::BattlePrepare;
    _on_ground_layer->removeAllChildren();
    _below_object_layer->removeAllChildren();
    _object_layer->removeAllChildren();
    _over_object_layer->removeAllChildren();
    _effect_layer->removeAllChildren();
    _float_layer->reset();
    _toast_layer->removeAllChildren();
    _skill_ui_layer->removeAllSkillNodes();
    _story_layer->reset();
    
    //clear units
    _player_units.clear();
    _alive_units.clear();
    _dead_units.clear();
    
    _bullets.clear();
    _towers.clear();
    _buildings.clear();
    _block_nodes.clear();
    
    _next_deploy_id = 0;
}

void BattleLayer::updateFrame( float delta ) {
    if( _state == eBattleState::BattleRunning || _state == eBattleState::BattleWin || _state == eBattleState::BattleLose ) {
        //update skill
        SkillCache::getInstance()->updateFrame( delta );
        _skill_ui_layer->updateFrame( delta );
        
        //handle dead units which need to be removed from battle
        //handle directly disappear units
        UnitMap alive_units = this->getAliveUnits();
        for( auto pair : alive_units ) {
            UnitNode* unit = pair.second;
            
            if( unit->getUnitState() == eUnitState::Dead ) {
                this->onUnitDying( unit );
            }
        }
        
        UnitMap dead_unit_map = this->getDeadUnits();
        for( auto pair : dead_unit_map ) {
            UnitNode* unit = pair.second;
            
            if( unit->getUnitState() == eUnitState::Dead ) {
                this->onUnitDisappear( unit );
            }
        }
        
        this->updateBuildings( delta );
        this->updateTowers( delta );
        this->updateBlocks( delta );
        
        //handle newly dying units which need to be removed from alive list
        UnitMap unit_map = this->getAliveUnits();
        for( auto pair : unit_map ) {
            UnitNode* unit = pair.second;
            
            if( unit->getUnitState() == eUnitState::Dying ) {
                this->onUnitDying( unit );
            }
        }
        
        //update bullets
        this->updateBullets( delta );
        
        //handle alive units
        for( auto pair : _alive_units ) {
            UnitNode* unit = pair.second;
            unit->updateFrame( delta );
        }
        
        for( auto pair : _dead_units ) {
            UnitNode* unit = pair.second;
            unit->updateFrame( delta );
        }
        
        //recycle effects
        Vector<Node*> effect_nodes = _effect_layer->getChildren();
        for( auto node : effect_nodes ) {
            UnitNodeComponent* component = dynamic_cast<UnitNodeComponent*>( node );
            if( component ) {
                if( component->shouldRecycle() ) {
                    component->removeFromParent();
                }
                else {
                    component->updateFrame( delta );
                }
            }
        }
        
        Vector<Node*> onground_effect_nodes = _on_ground_layer->getChildren();
        for( auto node : onground_effect_nodes ) {
            UnitNodeComponent* component = dynamic_cast<UnitNodeComponent*>( node );
            if( component ) {
                if( component->shouldRecycle() ) {
                    component->removeFromParent();
                }
                else {
                    component->updateFrame( delta );
                }
            }
        }
        
        this->reorderObjectLayer();
        this->adjustCamera();
        
        //check drop items
        for( auto pair : _player_units ) {
            UnitNode* unit = pair.second;
            Point world_pos = unit->getParent()->convertToWorldSpace( unit->getPosition() );
            
            auto itr = _drop_items.begin();
            while( itr != _drop_items.end() ) {
                DropItem* item = *itr;
                Point local_pos = item->convertToNodeSpace( world_pos );
                Rect rect = Rect( 0, 0, item->getContentSize().width, item->getContentSize().height );
                if( rect.containsPoint( local_pos ) ) {
                    _map_logic->obtainItem( item->getItemId(), item->getCount() );
                    itr = _drop_items.erase( itr );
                    item->removeFromParentAndCleanup( true );
                }
                else {
                    ++itr;
                }
            }
        }
        
        
        if( _should_show_fog ) {
            this->updateFogSprite();
        }
    }
    else if( _state == eBattleState::BattleStory ) {
        _story_layer->updateFrame( delta );
    }
    
    if( _state == BattleRunning ) {
        _game_time += delta;
        _map_logic->updateFrame( delta );
        _map_logic->updateGameTime( _game_time );
        this->checkState();
    }
    else if( _state == BattleWin || _state == BattleLose ) {
        _result_elapse += delta;

        if( _state == BattleLose || _result_elapse > 5.0f ) {
            if( _state == BattleWin ) {
                _battle_menu_layer->showResultPanel( true, _map_logic->getDropedItems() );
            }
            else {
                _battle_menu_layer->showResultPanel( false, ValueMap() );
            }
            this->changeState( eBattleState::BattleResult );
        }
    }
}

void BattleLayer::startBattle() {
    if( _state != eBattleState::BattleRunning ) {
        _state = eBattleState::BattleRunning;
    }
}

void BattleLayer::pauseBattle() {
    if( _state == eBattleState::BattleRunning ) {
        _state = eBattleState::BattlePaused;
        this->pause();
    }
}

void BattleLayer::resumeBattle() {
    if( _state == eBattleState::BattlePaused ) {
        _state = eBattleState::BattleRunning;
        this->resume();
    }
}

void BattleLayer::restartBattle() {
    this->reset();
    this->setup();
}

void BattleLayer::quitBattle() {
}

void BattleLayer::setMapLogic( MapLogic* map_logic ) {
    CC_SAFE_RETAIN( map_logic );
    CC_SAFE_RELEASE( _map_logic );
    _map_logic = map_logic;
}

void BattleLayer::setMapData( MapData* map_data ) {
    CC_SAFE_RETAIN( map_data );
    CC_SAFE_RELEASE( _map_data );
    _map_data = map_data;
}

UnitNode* BattleLayer::getLeaderUnit() {
    std::vector<std::string> keys = _player_units.keys();
    if( keys.size() > 0 ) {
        std::sort( keys.begin(), keys.end() );
        return _player_units.at( keys.at( 0 ) );
    }
    
    return nullptr;
}

void BattleLayer::changeState( eBattleState new_state ) {
    if( new_state == eBattleState::BattleLose || new_state == eBattleState::BattleWin ) {
        if( _state != eBattleState::BattleRunning ) {
            return;
        }
    }
    this->setState( new_state );
}

bool BattleLayer::checkState() {
    if( _state == eBattleState::BattleWin || _state == eBattleState::BattleLose ) {
        _result_elapse = 0;
        return false;
    }
    return true;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsByCondition( eTargetCamp camp, const std::vector<std::string>& tags, const cocos2d::Point& center, float range ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        Point unit_pos = unit->getPosition();
        if( unit->getTargetCamp() == camp && Math::isPositionInRange( unit_pos, center, range ) ) {
            bool qualified = true;
            for( auto str : tags ) {
                if( !unit->hasUnitTag( str ) ) {
                    qualified = false;
                    break;
                }
            }
            
            if( qualified ) {
                ret.pushBack( unit );
            }
        }
    }
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveOpponents( eTargetCamp camp ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            ret.pushBack( unit );
        }
    }
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsInRange( const cocos2d::Point& center, float range ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        Point unit_pos = unit->getPosition();
        if( Math::isPositionInRange( unit->getPosition(), center, range + unit->getUnitData()->collide ) ) {
            ret.pushBack( unit );
        }
    }
    
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsInRoundRange( const cocos2d::Point& center, float radius ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        Point unit_pos = unit->getPosition();
        if( Math::isPositionInRoundRange( unit->getPosition(), center, radius + unit->getUnitData()->collide ) ) {
            ret.pushBack( unit );
        }
    }
    
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsByCamp( eTargetCamp camp ) {
    cocos2d::Vector<UnitNode*> ret;

    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getTargetCamp() == camp ) {
            ret.pushBack( unit );
        }
    }
    
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveAllyInRange( eTargetCamp camp, const cocos2d::Point& center, float radius ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( !unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( Math::isPositionInRange( unit->getPosition(), center, radius + unit->getUnitData()->collide ) ) {
                ret.pushBack( unit );
            }
        }
    }
    
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsByTag( const std::string& tag ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->hasUnitTag( tag ) ) {
            ret.pushBack( unit );
        }
    }
    
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsByName( const std::string& name ) {
    cocos2d::Vector<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getUnitData()->name == name ) {
            ret.pushBack( unit );
        }
    }
    
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveOpponentsInRange( eTargetCamp camp, const cocos2d::Point& center, float radius ) {
    cocos2d::Vector<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( Math::isPositionInRange( unit->getPosition(), center, radius + unit->getUnitData()->collide ) ) {
                ret.pushBack( unit ); 
            }
        }
    }
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveOpponentsInRange( eTargetCamp camp, const cocos2d::Point& init_pos, const cocos2d::Point& center, float radius ) {
    cocos2d::Vector<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( Math::isPositionInRange( unit_pos, center, radius + unit->getUnitData()->collide ) && !Terrain::getInstance()->isBlocked( init_pos, unit_pos ) ) {
                ret.pushBack( unit );
            }
        }
    }
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveOpponentsInRoundRange( eTargetCamp camp, const cocos2d::Point& init_pos, const cocos2d::Point& center, float radius ) {
    cocos2d::Vector<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( Math::isPositionInRoundRange( unit_pos, center, radius + unit->getUnitData()->collide ) && !Terrain::getInstance()->isBlocked( init_pos, unit_pos ) ) {
                ret.pushBack( unit );
            }
        }
    }
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveOpponentsInSector( eTargetCamp camp, const cocos2d::Point& center, const cocos2d::Point& dir, float radius, float angle ) {
    cocos2d::Vector<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( Math::isPointInSector( unit_pos, center, dir, radius + unit->getUnitData()->collide, angle ) ) {
                ret.pushBack( unit );
            }
        }
    }
    return ret;
}

cocos2d::Vector<UnitNode*> BattleLayer::getAliveUnitsByCampAndSightGroup( eTargetCamp camp, const std::string& sight_group ) {
    cocos2d::Vector<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getTargetCamp() == camp && unit->getSightGroup() == sight_group ) {
            ret.pushBack( unit );
        }
    }
    return ret;
}

cocos2d::Vector<TowerNode*> BattleLayer::getAliveTowersInRange( eTargetCamp camp, const cocos2d::Point& center, float range ) {
    cocos2d::Vector<TowerNode*> ret;
    for( auto pair : _towers ) {
        TowerNode* tower = pair.second;
        if( tower->isAlive() && tower->isFoeOfCamp( camp ) ) {
            Point tower_pos = tower->getPosition();
            if( Math::isPositionInRange( tower_pos, center, range + tower->getTargetData()->collide ) ) {
                ret.pushBack( tower );
            }
        }
    }
    return ret;
}

cocos2d::Vector<TowerNode*> BattleLayer::getAliveTowersInRange( eTargetCamp camp, const cocos2d::Point& init_pos, const cocos2d::Point& center, float range ) {
    cocos2d::Vector<TowerNode*> ret;
    for( auto pair : _towers ) {
        TowerNode* tower = pair.second;
        if( tower->isAlive() && tower->isFoeOfCamp( camp ) ) {
            Point tower_pos = tower->getPosition();
            if( Math::isPositionInRange( tower_pos, center, range + tower->getTargetData()->collide ) && !Terrain::getInstance()->isBlocked( init_pos, tower_pos ) ) {
                ret.pushBack( tower );
            }
        }
    }
    return ret;
}

TargetNode* BattleLayer::getAliveTargetByDeployId( int deploy_id ) {
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getDeployId() == deploy_id ) {
            return unit;
        }
    }
    
    for( auto pair : _towers ) {
        TowerNode* tower = pair.second;
        if( tower->getDeployId() == deploy_id ) {
            return tower;
        }
    }
    
    return nullptr;
}

void BattleLayer::addBlockNode( BlockNode* block_node, eBattleSubLayer layer ) {
    block_node->setDeployId( BattleLayer::getNextDeployId() );
    _block_nodes.insert( block_node->getDeployId(), block_node );
    this->addToLayer( block_node, layer, block_node->getPosition(), this->zorderForPositionOnObjectLayer( block_node->getPosition() ) );
}

void BattleLayer::removeBlockNode( BlockNode* block_node ) {
    block_node->removeFromParent();
    _block_nodes.erase( block_node->getDeployId() );
}

BlockNode* BattleLayer::getBlockNode( const std::string& name ) {
    for( auto pair : _block_nodes ) {
        BlockNode* block_node = pair.second;
        if( block_node->getBlockName() == name ) {
            return block_node;
        }
    }
    return nullptr;
}

bool BattleLayer::addBullet( int key, BulletNode* bullet, eBattleSubLayer layer ) {
    std::string k = Utils::stringFormat( "%d", key );
    auto itr = _bullets.find( k );
    if( itr == _bullets.end() ) {
        _bullets.insert( k, bullet );
        this->addToLayer( bullet, layer, bullet->getPosition(), 0 );
        return true;
    }
    return false;
}

void BattleLayer::removeBullet( int key ) {
    auto itr = _bullets.find( Utils::stringFormat( "%d", key ) );
    if( itr != _bullets.end() ) {
        BulletNode* node = itr->second;
        _bullets.erase( itr );
        node->removeFromParent();
    }
}

void BattleLayer::updateBullets( float delta ) {
    auto itr = _bullets.begin();
    while( itr != _bullets.end() ) {
        BulletNode* b = itr->second;
        if( b->shouldRecycle() ) {
            itr = _bullets.erase( itr );
            b->removeFromParent();
        }
        else {
            b->updateFrame( delta );
            ++itr;
        }
    }
}

void BattleLayer::updateTowers( float delta ) {
    for( auto pair : _towers ) {
        TowerNode* tower = pair.second;
        if( tower->isAlive() ) {
            tower->updateFrame( delta );
        }
    }
}

void BattleLayer::updateBlocks( float delta ) {
    for( auto pair : _block_nodes ) {
        BlockNode* block = pair.second;
        block->updateFrame( delta );
    }
}

void BattleLayer::updateBuildings( float delta ) {
    for( auto pair : _buildings ) {
        BuildingNode* building = pair.second;
        if( building->isEnabled() ) {
            building->updateFrame( delta );
        }
    }
}

void BattleLayer::onUnitAppear( UnitNode* unit ) {
    eTargetCamp camp = unit->getTargetCamp();
    if( camp == eTargetCamp::Player ) {
        _player_units.insert( Utils::stringFormat( "%d", unit->getDeployId() ), unit );
        _skill_ui_layer->addSkillNode( unit );
    }
    _alive_units.insert( Utils::stringFormat( "%d", unit->getDeployId() ), unit );
    
    _map_logic->onTargetNodeAppear( unit );
    unit->appear();
    if( unit->hasUnitTag( "boss" ) ) {
        //vibrate
        this->vibrate();
        
        _skill_ui_layer->showBossPanel();
        _skill_ui_layer->setBossInfo( unit->getUnitData() );
    }
}

void BattleLayer::onUnitDying( UnitNode* unit ) {
    SkillCache::getInstance()->removeSkillOfOwner( unit );
    for( auto pair : _alive_units ) {
        if( pair.second->getChasingTarget() == unit ) {
            pair.second->setChasingTarget( nullptr );
        }
    }
    std::string key = Utils::stringFormat( "%d", unit->getDeployId() );
    if( unit->getTargetCamp() == eTargetCamp::Player ) {
        _player_units.erase( key );
        _skill_ui_layer->removeSkillNode( unit );
    }
    _alive_units.erase( key );
    _dead_units.insert( key, unit );
    this->clearChasingTarget(  unit );
    _map_logic->onTargetNodeDead( unit );
    
    if( unit->hasUnitTag( "leader" ) ) {
        UnitNode* leader = this->getLeaderUnit();
        if( leader ) {
            leader->addUnitTag( "leader" );
            UnitNode::ItemMap& items = unit->getItems();
            for( auto pair : items ) {
                leader->addItem( pair.second );
            }
        }
    }
    if( unit->hasUnitTag( "boss" ) ) {
        _skill_ui_layer->hideBossPanel();
        Director::getInstance()->setTimeScale( 0.2 );
        _slow_elapse = 0;
        this->schedule( CC_CALLBACK_1( BattleLayer::slowTimeUpdate, this ), "slow_time_update" );
    }
}

void BattleLayer::onUnitDisappear( UnitNode* unit ) {
    std::string key = Utils::stringFormat( "%d", unit->getDeployId() );
    _dead_units.erase( key );
    _map_logic->onTargetNodeDisappear( unit );
    int count = _map_logic->getUnitDisappearCountByCamp( (int)eTargetCamp::Enemy );
    _skill_ui_layer->setKilled( count );
    unit->removeFromParent();
}

void BattleLayer::onUnitMoved( UnitNode* unit ) {
    _map_logic->onUnitMoved( unit );
}

bool BattleLayer::isPositionInVision( const cocos2d::Point& pos ) {
    return false;
}

void BattleLayer::addToObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder ) {
    node->setPosition( pos );
    _object_layer->addChild( node, local_zorder );
}

void BattleLayer::addToEffectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder ) {
    node->setPosition( pos );
    _effect_layer->addChild( node, local_zorder );
}

void BattleLayer::addToBelowObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder ) {
    node->setPosition( pos );
    _below_object_layer->addChild( node, local_zorder );
}

void BattleLayer::addToOverObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder ) {
    node->setPosition( pos );
    _over_object_layer->addChild( node, local_zorder );
}

void BattleLayer::addToOnGroundLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder ) {
    node->setPosition( pos );
    _on_ground_layer->addChild( node, local_zorder );
}

void BattleLayer::addToFloatLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder ) {
    node->setPosition( pos );
    _float_layer->addChild( node, local_zorder );
}

void BattleLayer::addToLayer( cocos2d::Node* node, eBattleSubLayer layer, const cocos2d::Point& pos, int local_zorder ) {
    switch( layer ) {
        case eBattleSubLayer::OnGroundLayer:
            this->addToOnGroundLayer( node, pos, local_zorder );
            break;
        case eBattleSubLayer::ObjectLayer:
            this->addToObjectLayer( node, pos, local_zorder );
            break;
        case eBattleSubLayer::EffectLayer:
            this->addToEffectLayer( node, pos, local_zorder );
            break;
        default:
            break;
    }
}

void BattleLayer::deployUnit( UnitNode* unit, const cocos2d::Point& pos, const std::string& sight_group ) {
    int deploy_id = this->getNextDeployId();
    unit->setDeployId( deploy_id );
    unit->setSightGroup( sight_group );
    unit->setBornPos( pos );
    this->addToObjectLayer( unit, pos, this->zorderForPositionOnObjectLayer( pos ) );
    
    this->onUnitAppear( unit );
}

void BattleLayer::deployUnits( const cocos2d::Vector<UnitNode*>& units, const cocos2d::Rect& area, const std::string& sight_group ) {
    for( auto unit : units ) {
        Point pos = this->getAvailablePosition( unit->getUnitData()->collide, area );
        this->deployUnit( unit, pos, sight_group );
    }
}

void BattleLayer::deployTower( TowerNode* tower, const cocos2d::Point& pos, eBattleSubLayer layer ) {
    int deploy_id = this->getNextDeployId();
    tower->setDeployId( deploy_id );
    this->addToLayer( tower, layer, pos, this->zorderForPositionOnObjectLayer( pos ) );
    _towers.insert( deploy_id, tower );
}

void BattleLayer::deployBuilding( BuildingNode* building, const cocos2d::Point& pos, eBattleSubLayer layer ) {
    int deploy_id = this->getNextDeployId();
    building->setDeployId( deploy_id );
    this->addToLayer( building, layer, pos, this->zorderForPositionOnObjectLayer( building->getCenter() ) );
    _buildings.insert( deploy_id, building );
}

void BattleLayer::adjustCamera() {
    UnitNode* leader_unit = this->getLeaderUnit();
    if( leader_unit ) {
        this->centerCameraToPosition( leader_unit->getPosition() );
    }
}

void BattleLayer::centerCameraToPosition( const cocos2d::Point& pos ) {
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    Size visible_size = Director::getInstance()->getVisibleSize();
    
    float new_pos_x = visible_origin.x + visible_size.width / 2 - pos.x;
    float new_pos_y = visible_origin.y + visible_size.height / 2 - pos.y;
    
    if( new_pos_x > _max_map_position.x ) {
        new_pos_x = _max_map_position.x;
    }
    else if( new_pos_x < _min_map_position.x ) {
        new_pos_x = _min_map_position.x;
    }
    
    if( new_pos_y > _max_map_position.y ) {
        new_pos_y = _max_map_position.y;
    }
    else if( new_pos_y < _min_map_position.y ) {
        new_pos_y = _min_map_position.y;
    }
    
    _tmx_map->setPosition( new_pos_x, new_pos_y );
}

int BattleLayer::getNextDeployId() {
    if( ++_next_deploy_id == 0x00ffffff ) {
        _next_deploy_id = 1;
    }
    return _next_deploy_id;
}

cocos2d::Point BattleLayer::getAvailablePosition( float radius, const cocos2d::Rect& region ) {
    cocos2d::Rect temp_region = region;
    
#define RETRY_TIMES_BEFORE_ENLARGE (10)
#define MAX_ENLARGE_TIMES (50)
    int enlarge_time = 0;
    
    while( true ) {
        for( int t = 0; t != RETRY_TIMES_BEFORE_ENLARGE; ++t ) {
            float y_pos = temp_region.getMinY() + Utils::randomNumber( (int)temp_region.size.height ) - 1;
            float x_pos = temp_region.getMinX() + Utils::randomNumber( (int)temp_region.size.width ) - 1;
            cocos2d::Point pt( x_pos, y_pos );
            if( isPositionOK( pt, radius ) ) {
                return pt;
            }
        }
        
        enlarge_time++;
        if( enlarge_time > MAX_ENLARGE_TIMES ) {
            char buf[500];
            sprintf(buf, "getAvailablePosition failed. rect: x:%.2f, y: %.2f, width: %.2f, height: %.2f",
                    region.getMinX(), region.getMinY(), region.size.width, region.size.height);
            cocos2d::log("%s", buf);
            break;
        }
        
        float step = radius * 2.0f;
        temp_region.setRect( temp_region.getMinX() - step, temp_region.getMinY() - step, temp_region.size.width + step * 2.0f, temp_region.size.height + step * 2.0f );
    }

    return Point::ZERO;
}

bool BattleLayer::isPositionOK( cocos2d::Point pos, float radius ) {
    auto mesh = Terrain::getInstance()->getMeshByUnitRadius( radius );
    if( mesh->findPolygonByPoint( pos) ) {
        std::vector<Collidable*> collidables;
        mesh->getNearbyBorders( pos, radius, collidables );
        
        for( auto u : collidables ) {
            if( u->willCollide( pos, radius ) ) {
                return false;
            }
        }
        
        for( auto id_u : _alive_units ) {
            if( id_u.second->willCollide( pos, radius ) ) {
                return false;
            }
        }
        
        return true;
    }
    return false;
}

void BattleLayer::clearChasingTarget( TargetNode* target ) {
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getChasingTarget() == target ) {
            unit->setChasingTarget( nullptr );
        }
    }
}

void BattleLayer::startStory( const cocos2d::ValueMap& story_data ) {
    _story_layer->setVisible( true );
    _story_layer->setStoryData( story_data );
    _story_layer->setEnabled( true );
    _state = eBattleState::BattleStory;
}

void BattleLayer::endStory( UIStoryLayer* story ) {
    _story_layer->setVisible( false );
    _map_logic->onStoryChange( story->getStoryName(), STORY_STATE_END );
    _state = eBattleState::BattleRunning;
}

void BattleLayer::dropItem( DropItem* item, const cocos2d::Point& pos, eBattleSubLayer layer ) {
    item->setPosition( pos );
    _drop_items.pushBack( item );
    this->addToLayer( item, layer, pos, 0 );
}

void BattleLayer::vibrate() {
    AudioManager::getInstance()->vibrate();
    Point old_pos = _tmx_map->getPosition();
    MoveBy* move_1 = MoveBy::create( 0.1f, Point( -25.0f, 25.0f ) );
    MoveBy* move_2 = MoveBy::create( 0.1f, Point( 50.0f, -50.0f ) );
    MoveBy* move_3 = MoveBy::create( 0.1f, Point( -50.0f, 50.0f ) );
    MoveBy* move_4 = MoveBy::create( 0.1f, Point( 50.0f, -50.0f ) );
    MoveBy* move_5 = MoveBy::create( 0.1f, Point( -50.0f, 50.0f ) );
    MoveBy* move_6 = MoveBy::create( 0.1f, Point( 25.0f, -25.0f ) );
    MoveTo* move_back = MoveTo::create( 0.1f, old_pos );
    Sequence* seq = Sequence::create( move_1, move_2, move_3, move_4, move_5, move_6, move_back, nullptr );
    _tmx_map->runAction( seq );
}

void BattleLayer::slowTimeUpdate( float delta ) {
    _slow_elapse += delta;
    if( _slow_elapse > 0.2f ) {
        Director::getInstance()->setTimeScale( 1.0 );
        this->unschedule( "slow_time_update" );
    }
}

void BattleLayer::addHint( HintNode* hint_node, eBattleSubLayer layer ) {
    this->addToLayer( hint_node, layer, hint_node->getPosition(), this->zorderForPositionOnObjectLayer( hint_node->getPosition() ) );
    _hint_node_vec.pushBack( hint_node );
}

void BattleLayer::setHintVisibleByName( const std::string& name, bool visible ) {
    for( auto hint : _hint_node_vec ) {
        std::string hint_name = hint->getHintName();
        if( hint_name == name ) {
            hint->setVisible( visible );
        }
    }
}

//private methods

void BattleLayer::parseMapElementWithData( const TMXObjectGroup* group, const Value& data, eBattleSubLayer layer ) {
    const ValueMap& obj_properties = data.asValueMap();
    int gid = 0;
    std::string type = "";
    
    auto sitr = obj_properties.find( "gid" );
    if( sitr != obj_properties.end() ) {
        gid = sitr->second.asInt();
    }
    
    sitr = obj_properties.find( "type" );
    if( sitr != obj_properties.end() ) {
        type = sitr->second.asString();
    }
    
    ValueMap grid_properties;
    
    if( gid != 0 ) {
        bool flipped_horizontally = false;
        bool flipped_vertically = false;
        bool flipped_diagonally = false;
        if( ( gid & kTMXTileHorizontalFlag ) != 0 ) {
            flipped_horizontally = true;
            gid &= 0x7fffffff;
        }
        if( ( gid & kTMXTileVerticalFlag ) != 0 ) {
            flipped_vertically = true;
            gid &= 0xbfffffff;
        }
        if( ( gid & kTMXTileDiagonalFlag ) != 0 ) {
            flipped_diagonally = true;
            gid &= 0xdfffffff;
        }
        
        Value properties = _tmx_map->getPropertiesForGID( gid );
        grid_properties = properties.asValueMap();
        grid_properties["flipped_horizontally"] = Value( flipped_horizontally );
        grid_properties["flipped_vertically"] = Value( flipped_vertically );
        grid_properties["flipped_diagonally"] = Value( flipped_diagonally );
    }
    
    if( type == "SpriteBlockNode" ) {
        std::string name = obj_properties.at( "name" ).asString() + "_collide";
        ValueMap boundary = group->getObject( name );
        boundary["map_height"] = Value( _tmx_map->getContentSize().height );
        if( !boundary.empty() ) {
            grid_properties["boundary"] = Value( boundary );
            BlockNode* block_node = BlockNode::create( this, grid_properties, obj_properties );
            this->addBlockNode( block_node, layer );
        }
    }
    else if( type == "HintNode" ) {
        std::string name = obj_properties.at( "name" ).asString();
        Sprite* sp = _map_data->spriteFromObject( _tmx_map, data, true );
        
        HintNode* hint_node = HintNode::create( name, sp );
        hint_node->setVisible( false );
        this->addHint( hint_node, layer );
    }
    else if( type == "GroupSpineBlockNode" ) {
        std::string name = obj_properties.at( "name" ).asString();
        std::string boundary_name = name + "_collide";
        ValueMap boundary = group->getObject( boundary_name );
        if( !boundary.empty() ) {
            boundary["map_height"] = Value( _tmx_map->getContentSize().height );
            grid_properties["boundary"] = Value( boundary );
            
            BlockNode* block_node = this->getBlockNode( name );
            if( block_node == nullptr ) {
                block_node = BlockNode::create( this, grid_properties, obj_properties );
                this->addBlockNode( block_node, layer );
            }
            else {
                GroupSpineBlockNode* group_block = dynamic_cast<GroupSpineBlockNode*>( block_node );
                group_block->appendSpineNode( grid_properties, obj_properties );
            }
        }
    }
    else if( type.find( "tower" ) != std::string::npos ) {
        ValueMap tower_data = obj_properties;
        sitr = obj_properties.find( "level" );
        if( sitr != obj_properties.end() ) {
            tower_data["level"] = sitr->second;
        }
        else {
            tower_data["level"] = Value( 1 );
        }

        std::string name = obj_properties.at( "name" ).asString() + "_collide";
        ValueMap boundary = group->getObject( name );
        boundary["map_height"] = Value( _tmx_map->getContentSize().height );
        if( !boundary.empty() ) {
            tower_data["boundary"] = Value( boundary );
        }
        
        TowerNode* tower = TowerNode::create( this, tower_data );
        this->deployTower( tower, tower->getPosition(), layer );
    }
    else if( type.find( "thorn" ) != std::string::npos ) {
        ValueMap tower_data = obj_properties;
        sitr = obj_properties.find( "level" );
        if( sitr != obj_properties.end() ) {
            tower_data["level"] = sitr->second;
        }
        else {
            tower_data["level"] = Value( 1 );
        }
        
        std::string name = obj_properties.at( "name" ).asString() + "_collide";
        ValueMap boundary = group->getObject( name );
        boundary["map_height"] = Value( _tmx_map->getContentSize().height );
        if( !boundary.empty() ) {
            tower_data["boundary"] = Value( boundary );
        }
        
        ThornNode* tower = ThornNode::create( this, tower_data );
        this->deployTower( tower, tower->getPosition(), layer );
    }
    else if( type.find( "BuildingNode" ) != std::string::npos ) {
        std::string name = obj_properties.at( "name" ).asString() + "_collide";
        ValueMap boundary = group->getObject( name );
        boundary["map_height"] = Value( _tmx_map->getContentSize().height );
        if( !boundary.empty() ) {
            grid_properties["boundary"] = Value( boundary );
        }
        BuildingNode* building = BuildingNode::create( this, grid_properties, obj_properties );
        if( building ) {
            this->deployBuilding( building, building->getPosition(), layer );
        }
    }
    else if( type.empty() ) {
        Sprite* sp = _map_data->spriteFromObject( _tmx_map, data, true );
        if( sp ) {
            Point center = Point( sp->getContentSize().width / 2, sp->getContentSize().width / 3.4 ) + sp->getPosition();
            this->addToLayer( sp, layer, sp->getPosition(), this->zorderForPositionOnObjectLayer( center ) );
        }
    }
}

void BattleLayer::parseMapObjects() {
    const TMXObjectGroup* vision_group = _tmx_map->getObjectGroup( "vision" );
    if( vision_group ) {
        const ValueVector& objects = vision_group->getObjects();
        for( ValueVector::const_iterator itr = objects.begin(); itr != objects.end(); ++itr ) {
            this->parseMapElementWithData( vision_group, *itr, eBattleSubLayer::ObjectLayer );
        }
    }
    
    TMXObjectGroup* onground_group = _tmx_map->getObjectGroup( "onground" );
    if( onground_group ) {
        const ValueVector& onground_objects = onground_group->getObjects();
        for( auto itr = onground_objects.begin(); itr != onground_objects.end(); ++itr ) {
            this->parseMapElementWithData( onground_group, *itr, eBattleSubLayer::OnGroundLayer );
        }
    }
}

int BattleLayer::zorderForPositionOnObjectLayer( const cocos2d::Point& pos ) {
    return ( (int)-pos.x - (int)pos.y * (int)_tmx_map->getContentSize().width );
}

void BattleLayer::reorderObjectLayer() {
    for( auto pair : _bullets ) {
        BulletNode* bullet = pair.second;
        bullet->setLocalZOrder( this->zorderForPositionOnObjectLayer( bullet->getPosition() ) );
    }
    for( auto pair : _alive_units ) {
        UnitNode* unit_node = pair.second;
        unit_node->setLocalZOrder( this->zorderForPositionOnObjectLayer( unit_node->getPosition() ) );
    }
    for( auto pair : _dead_units ) {
        UnitNode* unit_node = pair.second;
        unit_node->setLocalZOrder( this->zorderForPositionOnObjectLayer( unit_node->getPosition() ) );
    }
}

void BattleLayer::updateFogSprite() {
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getTargetCamp() != eTargetCamp::Player ) {
            unit->setVisible( false );
        }
    }
    
    memset( _fog_tile_visible_array, 0, _fog_x_tile_count * _fog_y_tile_count );
    
    for( auto pair : _player_units ) {
        UnitNode* unit = pair.second;
        Point pos = unit->getParent()->convertToWorldSpace( unit->getPosition() );
        int view_range = unit->getTargetData()->view_range;
        
        int unit_x = pos.x / FOG_TILE_SIZE;
        int unit_y = pos.y / FOG_TILE_SIZE;
        
        int sx = MAX( 0, unit_x - view_range );
        int ex = MIN( _fog_x_tile_count - 1, unit_x + view_range );
        int sy = MAX( 0, unit_y - view_range );
        int ey = MIN( _fog_y_tile_count - 1, unit_y + view_range );
        
        for( int x = sx; x < ex; x++ ) {
            for( int y = sy; y < ey; y++ ) {
                int tag = y * _fog_x_tile_count + x;
                _fog_tile_visible_array[tag] |= 0x1;
                _fog_tile_visible_array[tag+1] |= 0x2;
                _fog_tile_visible_array[tag+_fog_x_tile_count] |= 0x4;
                _fog_tile_visible_array[tag+_fog_x_tile_count+1] |= 0x8;
            }
        }
        
        Vector<UnitNode*> candidates = this->getAliveOpponentsInRange( unit->getTargetCamp(), unit->getPosition(), unit->getPosition(), view_range * ( FOG_TILE_SIZE + 3 ) );
        for( auto u : candidates ) {
            u->setVisible( true );
        }
    }
    
    for( auto nd : _fog_node->getChildren() ) {
        Sprite* sp = dynamic_cast<Sprite*>( nd );
        int tag = sp->getTag();
        int flag = _fog_tile_visible_array[tag];
        int x = flag / 4;
        int y = flag % 4;
        sp->setTextureRect( Rect( x * FOG_TILE_SIZE, y * FOG_TILE_SIZE, FOG_TILE_SIZE, FOG_TILE_SIZE ) );
    }
}