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

using namespace cocos2d;

BattleLayer::BattleLayer() : _map_data( nullptr ), _map_logic( nullptr ) {
}

BattleLayer::~BattleLayer() {
    if( _map_data ) {
        _map_data->release();
    }
    if( _map_logic ) {
        _map_logic->release();
    }
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
        _map_data = map_data;
        _map_data->retain();
        
        std::set<float> all_collide_radius;
        all_collide_radius.insert( 30.0f );
        all_collide_radius.insert( 40.0f );
        all_collide_radius.insert( 50.0f );
        Terrain::getInstance()->parseMap( _map_data->getMapData(), all_collide_radius );
        
        cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
        cocos2d::Size size = Director::getInstance()->getVisibleSize();
    
        _tmx_map = map_data->generateTiledMapWithFlags( 3 );
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
        
        _ui_layer = UIBattleLayer::create( this );
        this->addChild( _ui_layer, eBattleSubLayer::BattleUILayer, eBattleSubLayer::BattleUILayer );
        
        _battle_menu_layer = UIBattleMenuLayer::create( this );
        this->addChild( _battle_menu_layer, eBattleSubLayer::BattleMenuLayer, eBattleSubLayer::BattleMenuLayer );
        
        _control_layer = UIControlLayer::create();
        this->addChild( _control_layer, eBattleSubLayer::ControlLayer, eBattleSubLayer::ControlLayer );
        
        this->setup();
        
        this->startBattle();
        
        this->schedule( CC_CALLBACK_1( BattleLayer::updateFrame, this ), "battle_update"  );
        
        return true;
    }while( 0 );
    return false;
}

void BattleLayer::setup() {
    this->reset();
    this->parseMapObjects();
    MapLogic* new_map_logic = MapLogic::retainedCreate( this );
    this->setMapLogic( new_map_logic );
    _map_logic->onMapInit();
    _game_time = 0;
}

void BattleLayer::reset() {
    _state = eBattleState::Prepare;
    _on_ground_layer->removeAllChildren();
    _below_object_layer->removeAllChildren();
    _object_layer->removeAllChildren();
    _over_object_layer->removeAllChildren();
    _effect_layer->removeAllChildren();
    _float_layer->reset();
    _toast_layer->removeAllChildren();
    _ui_layer->removeAllSkillNodes();
    
    //clear units
    _alive_units.clear();
    _dead_units.clear();
    
    _bullets.clear();
    
    _next_deploy_id = 0;
}

void BattleLayer::updateFrame( float delta ) {
    if( _state == Running ) {
        _game_time += delta;
        //handle dead units which need to be removed from battle
        UnitMap dead_unit_map = this->getDeadUnits();
        for( auto pair : dead_unit_map ) {
            UnitNode* unit = pair.second;
            
            if( unit->getUnitState() == eUnitState::Dead ) {
                this->onUnitDead( unit );
            }
        }
        
        //handle newly dying units which need to be removed from alive list
        UnitMap unit_map = this->getAliveUnits();
        for( auto pair : unit_map ) {
            UnitNode* unit = pair.second;
            
            if( unit->getUnitState() == eUnitState::Dying ) {
                this->onUnitDying( unit );
            }
        }
        
        //update bullets
        BulletMap bullets = _bullets;
        
        for( auto pair : bullets ) {
            BulletNode* b = pair.second;
            if( b->shouldRecycle() ) {
                this->removeBullet( b->getBulletId() );
            }
            else {
                b->updateFrame( delta );
            }
        }
        
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
        
        _map_logic->updateFrame( delta );
        this->reorderObjectLayer();
        this->adjustCamera();
    }
}

void BattleLayer::startBattle() {
    if( _state != eBattleState::Running ) {
        _state = eBattleState::Running;
    }
}

void BattleLayer::pauseBattle() {
    if( _state == eBattleState::Running ) {
        _state = eBattleState::Paused;
    }
}

void BattleLayer::resumeBattle() {
    if( _state == eBattleState::Paused ) {
        _state = eBattleState::Running;
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
    if( _map_logic ) {
        _map_logic->release();
    }
    _map_logic = map_logic;
}

UnitNode* BattleLayer::getLeaderUnit() {
    if( _player_units.begin() != _player_units.end() ) {
        return _player_units.begin()->second;
    }
    return nullptr;
}

void BattleLayer::changeState( eBattleState new_state ) {
    
}

std::list<UnitNode*> BattleLayer::getAliveOpponents( eUnitCamp camp ) {
    std::list<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            ret.push_back( unit );
        }
    }
    return ret;
}

std::list<UnitNode*> BattleLayer::getAliveUnitsByCamp( eUnitCamp camp ) {
    std::list<UnitNode*> ret;

    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getUnitCamp() == camp ) {
            ret.push_back( unit );
        }
    }
    
    return ret;
}

std::list<UnitNode*> BattleLayer::getAliveUnitsByTag( const std::string& tag ) {
    std::list<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->hasUnitTag( tag ) ) {
            ret.push_back( unit );
        }
    }
    
    return ret;
}

std::list<UnitNode*> BattleLayer::getAliveUnitsByName( const std::string& name ) {
    std::list<UnitNode*> ret;
    
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getUnitData()->name == name ) {
            ret.push_back( unit );
        }
    }
    
    return ret;
}

std::list<UnitNode*> BattleLayer::getAliveOpponentsInRange( eUnitCamp camp, const cocos2d::Point& center, float radius ) {
    std::list<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->isFoeOfCamp( camp ) ) {
            Point unit_pos = unit->getPosition();
            if( unit_pos.distance( center ) <= radius ) {
                ret.push_back( unit );
            }
        }
    }
    return ret;
}

std::list<UnitNode*> BattleLayer::getAliveUnitsByCampAndSightGroup( eUnitCamp camp, const std::string& sight_group ) {
    std::list<UnitNode*> ret;
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getUnitCamp() == camp && unit->getSightGroup() == sight_group ) {
            ret.push_back( unit );
        }
    }
    return ret;
}

UnitNode* BattleLayer::getAliveUnitByDeployId( int deploy_id ) {
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getDeployId() == deploy_id ) {
            return unit;
        }
    }
    return nullptr;
}

bool BattleLayer::addBullet( int key, BulletNode* bullet ) {
    if( _bullets.insert( std::make_pair( key, bullet ) ).second ) {
        this->addToEffectLayer( bullet, bullet->getPosition(), bullet->getLocalZOrder() );
        return true;
    }
    return false;
}

void BattleLayer::removeBullet( int key ) {
    auto itr = _bullets.find( key );
    if( itr != _bullets.end() ) {
        BulletNode* node = itr->second;
        _bullets.erase( itr );
        node->removeFromParent();
    }
}

void BattleLayer::onUnitAppear( UnitNode* unit ) {
    eUnitCamp camp = unit->getUnitCamp();
    if( camp == eUnitCamp::Player ) {
        _player_units.insert( std::make_pair( unit->getDeployId(), unit ) );
    }
    _alive_units.insert( std::make_pair( unit->getDeployId(), unit ) );
    
    _map_logic->onTargetNodeAppear( unit );
    unit->appear();
}

void BattleLayer::onUnitDying( UnitNode* unit ) {
    for( auto pair : _alive_units ) {
        if( pair.second->getChasingTarget() == unit ) {
            pair.second->setChasingTarget( nullptr );
        }
    }
    if( unit->getUnitCamp() == eUnitCamp::Player ) {
        _player_units.erase( unit->getDeployId() );
    }
    _alive_units.erase( unit->getDeployId() );
    _dead_units.insert( std::make_pair( unit->getDeployId(), unit ) );
    this->clearChasingTarget(  unit );
}

void BattleLayer::onUnitDead( UnitNode* unit ) {
    _dead_units.erase( unit->getDeployId() );
    _map_logic->onTargetNodeDisappear( unit );
    unit->removeFromParent();
}

bool BattleLayer::isPositionInVision( const cocos2d::Point& pos ) {
    return false;
}

void BattleLayer::addToObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, const cocos2d::Point& center ) {
    int zorder = this->zorderForPositionOnObjectLayer( center );
    node->setPosition( pos );
    node->setLocalZOrder( zorder );
    _object_layer->addChild( node );
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

void BattleLayer::deployUnit( UnitNode* unit, const cocos2d::Point& pos, const std::string& sight_group ) {
    int deploy_id = this->getNextDeployId();
    unit->setDeployId( deploy_id );
    unit->setSightGroup( sight_group );
    unit->setBornPos( pos );
    this->addToObjectLayer( unit, pos, pos );
    
    this->onUnitAppear( unit );
}

void BattleLayer::deployUnits( const std::list<UnitNode*>& units, const cocos2d::Rect& area, const std::string& sight_group ) {
    for( auto unit : units ) {
        Point pos = this->getAvailablePosition( unit->getUnitData()->collide, area );
        this->deployUnit( unit, pos, sight_group );
    }
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

void BattleLayer::clearChasingTarget( TargetNode* unit ) {
    for( auto pair : _alive_units ) {
        UnitNode* unit = pair.second;
        if( unit->getChasingTarget() == unit ) {
            unit->setChasingTarget( nullptr );
        }
    }
}

//private methods
void BattleLayer::parseMapObjects() {
    const ValueVector& objects = _tmx_map->getObjectGroup( "vision" )->getObjects();
    for( ValueVector::const_iterator itr = objects.begin(); itr != objects.end(); ++itr ) {
        const ValueMap& obj_properties = itr->asValueMap();
        int gid = obj_properties.at( "gid" ).asInt();
        Value properties = _tmx_map->getPropertiesForGID( gid );
        const ValueMap& grid_properties = properties.asValueMap();
        BuildingNode* building = BuildingNode::create( grid_properties, obj_properties );
        this->addToObjectLayer( building, building->getPosition(), building->getPosition() + building->getCenter() );
    }
}

int BattleLayer::zorderForPositionOnObjectLayer( const cocos2d::Point& pos ) {
    return ( (int)-pos.x - (int)pos.y * (int)_tmx_map->getContentSize().width );
}

void BattleLayer::reorderObjectLayer() {
    for( auto pair : _alive_units ) {
        UnitNode* unit_node = pair.second;
        unit_node->setLocalZOrder( this->zorderForPositionOnObjectLayer( unit_node->getPosition() ) );
    }
    for( auto pair : _dead_units ) {
        UnitNode* unit_node = pair.second;
        unit_node->setLocalZOrder( this->zorderForPositionOnObjectLayer( unit_node->getPosition() ) );
    }
}