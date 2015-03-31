//
//  Trigger.cpp
//  Boids
//
//  Created by chenyanjie on 3/26/15.
//
//

#include "Trigger.h"
#include "MapLogic.h"
#include "../unit/UnitNode.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

Trigger::Trigger() {
    
}

Trigger::~Trigger() {
    
}

//factory function
Trigger* Trigger::create( const cocos2d::ValueMap& data ) {
    Trigger* ret = nullptr;
    std::string trigger_type = data.at( "trigger_type" ).asString();
    log( "%s", trigger_type.c_str() );
    
    if( trigger_type == EVENT_TRIGGER_TYPE_MAP_INIT ) {
        ret = MapInitTrigger::create( data );
    }
    else if( trigger_type == EVENT_TRIGGER_TYPE_UNIT_CHANGE ) {
        std::string unit_state = data.at( "unit_state" ).asString();
        if( unit_state == UNIT_STATE_DEAD ) {
            ret = UnitDeadTrigger::create( data );
        }
        else if( unit_state == UNIT_STATE_ALIVE ) {
            ret = UnitAliveTrigger::create( data );
        }
    }
    else if( trigger_type == EVENT_TRIGGER_TYPE_UNIT_STAY ) {
        ret = UnitStayTrigger::create( data );
    }
    else if( trigger_type == EVENT_TRIGGER_TYPE_CONVERSATION_CHANGE ) {
        ret = ConversationChangeTrigger::create( data );
    }
    
    return ret;
}

bool Trigger::init( const cocos2d::ValueMap& data ) {
    _trigger_data = data;
    _trigger_type = data.at( "trigger_type" ).asString();
    _could_trigger = false;
    return true;
}

void Trigger::updateTrigger( const cocos2d::ValueMap& update_data ) {
    for( auto itr = update_data.begin(); itr != update_data.end(); ++itr ) {
        auto subitr = _trigger_data.find( itr->first );
        if( subitr != _trigger_data.end() ) {
            if( subitr->second != itr->second ) {
                return;
            }
        }
    }
    this->setCouldTrigger( true );
}

void Trigger::updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state ) {
    
}

void Trigger::updateFrame( float delta ) {
    
}

MapInitTrigger::MapInitTrigger() {
    
}

MapInitTrigger::~MapInitTrigger() {
    
}

MapInitTrigger* MapInitTrigger::create( const cocos2d::ValueMap& data ) {
    MapInitTrigger* ret = new MapInitTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MapInitTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    return true;
}

UnitChangeTrigger::UnitChangeTrigger() {
    
}

UnitChangeTrigger::~UnitChangeTrigger() {
    
}

UnitChangeTrigger* UnitChangeTrigger::create( const cocos2d::ValueMap& data ) {
    UnitChangeTrigger* ret = new UnitChangeTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitChangeTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    return true;
}

void UnitChangeTrigger::updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state ) {
}

UnitDeadTrigger::UnitDeadTrigger() {
    
}

UnitDeadTrigger::~UnitDeadTrigger() {
    
}

UnitDeadTrigger* UnitDeadTrigger::create( const cocos2d::ValueMap& data ) {
    UnitDeadTrigger* ret = new UnitDeadTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitDeadTrigger::init( const cocos2d::ValueMap& data ) {
    if( !UnitChangeTrigger::init( data ) ) {
        return false;
    }
    
    _current_count = 0;
    
    auto itr = _trigger_data.find( "trigger_count" );
    if( itr != _trigger_data.end() ) {
        _need_count = itr->second.asInt();
    }
    else {
        _need_count = 1;
    }
    
    return true;
}

void UnitDeadTrigger::updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state ) {
    if( _trigger_data.at( "unit_state" ).asString() != unit_state ) {
        return;
    }
    
    std::string source_type = _trigger_data.at( "source_type" ).asString();
    std::string source_value = _trigger_data.at( "source_value" ).asString();
    
    if( source_type == UNIT_SOURCE_TYPE ) {
        if( unit_node->getUnitCamp() == UnitNode::getCampByString( source_value ) ) {
            ++_current_count;
        }
    }
    else if( source_type == UNIT_SOURCE_TAG ) {
        if( unit_node->hasUnitTag( source_value ) ) {
            ++_current_count;
        }
    }
    else if( source_type == UNIT_SOURCE_NAME ) {
        if( unit_node->getUnitData()->name == source_value ) {
            ++_current_count;
        }
    }
    
    if( _current_count >= _need_count ) {
        this->setCouldTrigger( true );
    }
}

UnitAliveTrigger::UnitAliveTrigger() {
    
}

UnitAliveTrigger::~UnitAliveTrigger() {
    
}

UnitAliveTrigger* UnitAliveTrigger::create( const cocos2d::ValueMap& data ) {
    UnitAliveTrigger* ret = new UnitAliveTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitAliveTrigger::init( const cocos2d::ValueMap& data ) {
    if( !UnitChangeTrigger::init( data ) ) {
        return false;
    }
    
    return true;
}

void UnitAliveTrigger::updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state ) {
    if( unit_state != UNIT_STATE_DEAD ) {
        return;
    }
    
    std::string source_type = _trigger_data.at( "source_type" ).asString();
    std::string source_value = _trigger_data.at( "source_value" ).asString();
    
    if( source_type == UNIT_SOURCE_TYPE ) {
        if( (int)map_logic->getBattleLayer()->getAliveUnitsByCamp( UnitNode::getCampByString( source_value ) ).size() <= _need_count ) {
            this->setCouldTrigger( true );
        }
    }
    else if( source_type == UNIT_SOURCE_TAG ) {
        if( (int)map_logic->getBattleLayer()->getAliveUnitsByTag( source_value ).size() <= _need_count ) {
            this->setCouldTrigger( true );
        }
    }
    else if( source_type == UNIT_SOURCE_NAME ) {
        if( (int)map_logic->getBattleLayer()->getAliveUnitsByName( source_value ).size() <= _need_count ) {
            this->setCouldTrigger( true );
        }
    }
}

UnitStayTrigger::UnitStayTrigger() {
    
}

UnitStayTrigger::~UnitStayTrigger() {
    
}

UnitStayTrigger* UnitStayTrigger::create( const cocos2d::ValueMap& data ) {
    UnitStayTrigger* ret = new UnitStayTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitStayTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    
    _elapse = 0;
    _duration = data.at( "duration" ).asFloat();
    _is_active = false;
    return true;
}

void UnitStayTrigger::updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state ) {
    if( unit_state != UNIT_STATE_MOVE_TO ) {
        return;
    }
    
    std::string source_type = _trigger_data.at( "source_type" ).asString();
    std::string source_value = _trigger_data.at( "source_value" ).asString();
    std::string position_name = _trigger_data.at( "position_name" ).asString();
    
    if( source_type == UNIT_SOURCE_TAG && unit_node->hasUnitTag( source_value ) ) {
        BattleLayer* battle_layer = unit_node->getBattleLayer();
        const cocos2d::ValueMap& area = battle_layer->getMapData()->getAreaMapByPosition( unit_node->getPosition() );
        if( area.at( "name" ).asString() == position_name && !_is_active ) {
            if( _duration <= 0 ) {
                this->setCouldTrigger( true );
            }
            else {
                this->setActive( true );
            }
        }
        else {
            this->setActive( false );
            _elapse = 0;
        }
    }
}

void UnitStayTrigger::updateFrame( float delta ) {
    if( !_could_trigger && _is_active ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->setCouldTrigger( true );
        }
    }
}

ConversationChangeTrigger::ConversationChangeTrigger() {
    
}

ConversationChangeTrigger::~ConversationChangeTrigger() {
    
}

ConversationChangeTrigger* ConversationChangeTrigger::create( const cocos2d::ValueMap& data ) {
    ConversationChangeTrigger* ret = new ConversationChangeTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ConversationChangeTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    
    return true;
}

EventChangeTrigger::EventChangeTrigger() {
    
}

EventChangeTrigger::~EventChangeTrigger() {
    
}

EventChangeTrigger* EventChangeTrigger::create( const cocos2d::ValueMap& data ) {
    EventChangeTrigger* ret = new EventChangeTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EventChangeTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    return true;
}

VisionChangeTrigger::VisionChangeTrigger() {
    
}

VisionChangeTrigger::~VisionChangeTrigger() {
    
}

VisionChangeTrigger* VisionChangeTrigger::create( const cocos2d::ValueMap& data ) {
    VisionChangeTrigger* ret = new VisionChangeTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool VisionChangeTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    return true;
}

CustomTrigger::CustomTrigger() {
    
}

CustomTrigger::~CustomTrigger() {
    
}

CustomTrigger* CustomTrigger::create( const cocos2d::ValueMap& data ) {
    CustomTrigger* ret = new CustomTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CustomTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    
    return true;
}

GameChangeTrigger::GameChangeTrigger() {
    
}

GameChangeTrigger::~GameChangeTrigger() {
    
}

GameChangeTrigger* GameChangeTrigger::create( const cocos2d::ValueMap& data ) {
    GameChangeTrigger* ret = new GameChangeTrigger();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GameChangeTrigger::init( const cocos2d::ValueMap& data ) {
    if( !Trigger::init( data ) ) {
        return false;
    }
    return true;
}