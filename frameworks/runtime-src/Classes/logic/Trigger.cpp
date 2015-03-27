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
    
    if( trigger_type == EVENT_TRIGGER_TYPE_MAP_INIT ) {
        ret = MapInitTrigger::create( data );
    }
    else if( trigger_type == EVENT_TRIGGER_TYPE_UNIT_CHANGE ) {
        ret = UnitChangeTrigger::create( data );
    }
    else if( trigger_type == EVENT_TRIGGER_TYPE_CONVERSATION_CHANGE ) {
        ret = ConversationChangeTrigger::create( data );
    }
    
    return ret;
}

bool Trigger::init( const cocos2d::ValueMap& data ) {
    _trigger_data = data;
    _trigger_type = data.at( "trigger_type" ).asString();
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
    if( _trigger_data.at( "unit_state" ).asString() != unit_state ) {
        return;
    }

    std::string source_type = _trigger_data.at( "source_type" ).asString();
    std::string source_value = _trigger_data.at( "source_value" ).asString();
    std::string position_name = "";
    
    auto itr = _trigger_data.find( "position_name" );
    if( itr != _trigger_data.end() ) {
        position_name = itr->second.asString();
    }
    int trigger_count = 1;
    itr = _trigger_data.find( "trigger_count" );
    if( itr != _trigger_data.end() ) {
        trigger_count = itr->second.asInt();
    }
    
    eUnitCamp camp = unit_node->getUnitCamp();
    if( source_type == UNIT_SOURCE_TYPE && camp == UnitNode::getCampByString( source_value ) ) {
        if( unit_state == UNIT_STATE_APPEAR ) {
            if( trigger_count <= map_logic->getUnitAppearCountByCamp( (int)camp ) ) {
                this->setCouldTrigger( true );
            }
        }
        else if( unit_state == UNIT_STATE_DISAPPEAR ) {
            if( trigger_count <= map_logic->getUnitDisappearCountByCamp( (int)camp ) ) {
                this->setCouldTrigger( true );
            }
        }
        else if( unit_state == UNIT_STATE_MOVE_TO ) {
            BattleLayer* battle_layer = unit_node->getBattleLayer();
            MapData* map_data = unit_node->getBattleLayer()->getMapData();
            int in_area_count = 0;
            cocos2d::Vector<UnitNode*> units = battle_layer->getAliveUnitsByCamp( camp );
            if( units.size() == 0 ) {
                return;
            }
            for( auto u : units ) {
                ValueMap unit_area = map_data->getAreaMapByPosition( u->getPosition() );
                if( ( unit_area.find( "name" ) != unit_area.end() ) && ( unit_area.at( "name" ).asString() == position_name ) ) {
                    ++in_area_count;
                }
            }
            if( trigger_count <= in_area_count ) {
                this->setCouldTrigger( true );
            }
        }
    }
    else if( source_type == UNIT_SOURCE_TAG && unit_node->hasUnitTag( source_value ) ) {
        if( unit_state == UNIT_STATE_APPEAR ) {
            if( trigger_count <= map_logic->getUnitAppearCountByTag( source_value ) ) {
                this->setCouldTrigger( true );
            }
        }
        else if( unit_state == UNIT_STATE_DISAPPEAR ) {
            if( trigger_count <= map_logic->getUnitDisappearCountByTag( source_value ) ) {
                this->setCouldTrigger( true );
            }
        }
        else if( unit_state == UNIT_STATE_MOVE_TO ) {
            BattleLayer* battle_layer = unit_node->getBattleLayer();
            MapData* map_data = battle_layer->getMapData();
            int in_area_count = 0;
            cocos2d::Vector<UnitNode*> units = map_logic->getBattleLayer()->getAliveUnitsByTag( source_value );
            if( units.size() == 0 ) {
                return;
            }
            for( auto u : units ) {
                ValueMap unit_area = map_data->getAreaMapByPosition( u->getPosition() );
                if( ( unit_area.find( "name" ) != unit_area.end() ) && ( unit_area.at( "name" ).asString() == position_name ) ) {
                    ++in_area_count;
                }
            }
            if( trigger_count <= in_area_count ) {
                this->setCouldTrigger( true );
            }
        }
    }
    else if( source_type == UNIT_SOURCE_NAME && unit_node->getUnitData()->name == source_value ) {
        if( unit_state == UNIT_STATE_APPEAR ) {
            if( trigger_count <= map_logic->getUnitAppearCountByName( source_value ) ) {
                this->setCouldTrigger( true );
            }
        }
        else if( unit_state == UNIT_STATE_DISAPPEAR ) {
            if( trigger_count <= map_logic->getUnitDisappearCountByName( source_value ) ) {
                this->setCouldTrigger( true );
            }
        }
        else if( unit_state == UNIT_STATE_MOVE_TO ) {
            BattleLayer* battle_layer = unit_node->getBattleLayer();
            MapData* map_data = battle_layer->getMapData();
            int in_area_count = 0;
            cocos2d::Vector<UnitNode*> units = map_logic->getBattleLayer()->getAliveUnitsByName( source_value );
            if( units.size() == 0 ) {
                return;
            }
            for( auto u : units ) {
                ValueMap unit_area = map_data->getAreaMapByPosition( u->getPosition() );
                if( ( unit_area.find( "name" ) != unit_area.end() ) && ( unit_area.at( "name" ).asString() == position_name ) ) {
                    ++in_area_count;
                }
            }
            if( trigger_count <= in_area_count ) {
                this->setCouldTrigger( true );
            }
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