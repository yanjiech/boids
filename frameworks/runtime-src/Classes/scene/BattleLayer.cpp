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

BattleLayer::BattleLayer() : _map_data( nullptr ), _map_logic( nullptr ) {
}

BattleLayer::~BattleLayer() {
    CC_SAFE_RELEASE( _map_data );
    CC_SAFE_RELEASE( _map_logic );
}

BattleLayer* BattleLayer::create( MapData* map_data, bool is_pvp ) {
    BattleLayer* ret = new BattleLayer();
    if( ret && ret->init( map_data, is_pvp ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BattleLayer::init( MapData* map_data, bool is_pvp ) {
    if( !Layer::init() ) {
        return false;
    }
    do {
        this->setMapData( map_data );
        
        std::set<float> all_collide_radius;
        all_collide_radius.insert( 30.0f );
        all_collide_radius.insert( 40.0f );
        all_collide_radius.insert( 50.0f );
        Terrain::getInstance()->parseMap( _map_data->getMapData(), all_collide_radius );
        
        cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
        cocos2d::Size size = Director::getInstance()->getVisibleSize();
    
        _tmx_map = map_data->generateTiledMapWithFlags( 1 );
        _tmx_map->setPosition( origin );
        this->addChild( _tmx_map, 0, eBattleSubLayer::MapLayer );
        
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
        
        _draw_node = DrawNode::create();
        _tmx_map->addChild( _draw_node, 100000 );
        
        this->setup();
        
        this->startBattle();
        
        _map_logic->onMapInit();
        
        this->schedule( CC_CALLBACK_1( BattleLayer::updateFrame, this ), "battle_update"  );
        
        return true;
    }while( 0 );
    return false;
}

void BattleLayer::setup() {
    this->reset();
    this->parseMapObjects();
    MapLogic* new_map_logic = MapLogic::create( this );
    this->setMapLogic( new_map_logic );
    _game_time = 0;
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
    if( _state == eBattleState::BattleRunning ) {
        //update skill
        SkillCache::getInstance()->updateFrame( delta );
        _skill_ui_layer->updateFrame( delta );
        
        //handle dead units which need to be removed from battle
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
    }
    
    else if( _state == eBattleState::BattleStory ) {
        _story_layer->updateFrame( delta );
    }
    
    if( _state == BattleRunning ) {
        _game_time += delta;
        _map_logic->updateFrame( delta );
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
    Terrain::getInstance()->release();
}

void BattleLayer::setMapLogic( MapLogic* map_logic ) {
    CC_SAFE_RELEASE( _map_logic );
    _map_logic = map_logic;
    CC_SAFE_RETAIN( _map_logic );
}

void BattleLayer::setMapData( MapData* map_data ) {
    CC_SAFE_RELEASE( _map_data );
    _map_data = map_data;
    CC_SAFE_RETAIN( _map_data );
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
    this->setState( new_state );
    if( _state == eBattleState::BattleWin ) {
        _battle_menu_layer = UIBattleMenuLayer::create( this, "You Win! @ @!" );
        this->addChild( _battle_menu_layer, eBattleSubLayer::BattleMenuLayer, eBattleSubLayer::BattleMenuLayer );
        log( "game end: win" );
    }
    else if( _state == eBattleState::BattleLose ) {
        _battle_menu_layer = UIBattleMenuLayer::create( this, "You Lose! @ @!" );
        this->addChild( _battle_menu_layer, eBattleSubLayer::BattleMenuLayer, eBattleSubLayer::BattleMenuLayer );
        log( "game end: lose" );
    }
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

cocos2d::Vector<UnitNode*> BattleLayer::getAliveOpponentsInSector( eTargetCamp camp, const cocos2d::Point& center, const cocos2d::Point& dir, float radius, float angle ) {
    cocos2d::Vector<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( Math::isPointInSector( unit_pos, center, dir, unit->getUnitData()->collide, 120.0f ) || Math::isPointInSector( unit_pos, center, dir, radius + unit->getUnitData()->collide, angle ) ) {
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

bool BattleLayer::addBullet( int key, BulletNode* bullet ) {
    std::string k = Utils::stringFormat( "%d", key );
    auto itr = _bullets.find( k );
    if( itr == _bullets.end() ) {
        _bullets.insert( k, bullet );
        this->addToEffectLayer( bullet, bullet->getPosition(), 0 );
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
}

void BattleLayer::onUnitDying( UnitNode* unit ) {
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
}

void BattleLayer::onUnitDisappear( UnitNode* unit ) {
    std::string key = Utils::stringFormat( "%d", unit->getDeployId() );
    _dead_units.erase( key );
    _map_logic->onTargetNodeDisappear( unit );
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
#define MAX_ENLARGE_TIMES (50) //扩展30次还没位置的话，九成是地图有问题啊。。
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
        if( ( gid & FLIPPED_HORIZONTALLY ) != 0 ) {
            flipped_horizontally = true;
            gid &= 0x7fffffff;
        }
        if( ( gid & FLIPPED_VERTICALLY ) != 0 ) {
            flipped_vertically = true;
            gid &= 0xbfffffff;
        }
        if( ( gid & FLIPPED_DIAGONALLY ) != 0 ) {
            flipped_diagonally = true;
            gid &= 0xdfffffff;
        }
        
        Value properties = _tmx_map->getPropertiesForGID( gid );
        grid_properties = properties.asValueMap();
        grid_properties["flipped_horizontally"] = Value( flipped_horizontally );
        grid_properties["flipped_vertically"] = Value( flipped_vertically );
        grid_properties["flipped_diagonally"] = Value( flipped_diagonally );
    }
    
    if( type.find( "BlockNode" ) != std::string::npos ) {
        std::string name = obj_properties.at( "name" ).asString() + "_collide";
        ValueMap boundary = group->getObject( name );
        boundary["map_height"] = Value( _tmx_map->getContentSize().height );
        if( !boundary.empty() ) {
            grid_properties["boundary"] = Value( boundary );
            BlockNode* block_node = BlockNode::create( this, grid_properties, obj_properties );
            this->addBlockNode( block_node, layer );
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
            this->addToLayer( sp, layer, sp->getPosition(), this->zorderForPositionOnObjectLayer( sp->getPosition() ) );
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