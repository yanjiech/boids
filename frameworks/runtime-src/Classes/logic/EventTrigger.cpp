//
//  EventTrigger.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#include "EventTrigger.h"
#include "../util/CocosUtils.h"
#include "MapLogic.h"
#include "../unit/UnitNode.h"
#include "../constant/BoidsConstant.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

EventTrigger::EventTrigger() {
    
}

EventTrigger::~EventTrigger() {
    
}

EventTrigger* EventTrigger::create( const rapidjson::Value& event ) {
    EventTrigger* ret = new EventTrigger();
    if( ret && ret->init( event ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EventTrigger::init( const rapidjson::Value& event ) {
    _trigger_index = 0;
    _is_enabled = true;
    
    _logic_event = CocosUtils::jsonObjectToValueMap( event );
    ValueMap& trigger_meta = _logic_event["trigger_meta"].asValueMap();
    auto itr = trigger_meta.find( "trigger_relation" );
    if( itr == trigger_meta.end() ) {
        _relation = "then";
    }
    else {
        _relation = itr->second.asString();
    }
    itr = trigger_meta.find( "is_repeated" );
    if( itr == trigger_meta.end() ) {
        _is_repeat = false;
    }
    else {
        _is_repeat = itr->second.asBool();
    }
    
    ValueVector& triggers = _logic_event["triggers"].asValueVector();
    _total_trigger_count = triggers.size();
    for( auto itr = triggers.begin(); itr != triggers.end(); ++itr ) {
        itr->asValueMap().insert( std::make_pair( "fired", Value( false ) ) );
    }
    
    return true;
}

void EventTrigger::setEnabled( bool b ) {
    _is_enabled = b;
    _trigger_index = 0;
}

bool EventTrigger::canFire() {
    if( _relation == "then" ) {
        return moveOn();
    }
    else if( _relation == "or" ) {
        return true;
    }
    else {
        ValueVector& triggers = _logic_event["triggers"].asValueVector();
        for( auto itr = triggers.begin(); itr != triggers.end(); ++itr ) {
            if( itr->asValueMap().at( "fired" ).asBool() == false ) {
                return false;
            }
        }
        return true;
    }
}

bool EventTrigger::isValid() {
    return ( _trigger_index < _total_trigger_count ) && _is_enabled;
}

bool EventTrigger::moveOn() {
    return ++_trigger_index >= _total_trigger_count;
}

void EventTrigger::onSingleTriggerPass( int index, class MapLogic* map_logic, class UnitNode* unit_node ) {
    ValueVector& trigger_vector = _logic_event.at( "triggers" ).asValueVector();
    ValueMap& trigger_data = trigger_vector.at( index ).asValueMap();
    trigger_data["fired"] = Value( true );
    if( this->canFire() ) {
        map_logic->executeLogicEvent( this, unit_node );
        if( _is_repeat ) {
            _trigger_index = 0;
            for( auto itr = trigger_vector.begin(); itr != trigger_vector.end(); ++itr ) {
                itr->asValueMap()["fired"] = Value( false );
            }
        }
        else {
            this->setEnabled( false );
        }
    }
}

void EventTrigger::activateTriggerByConditions( const cocos2d::ValueMap& conditions, class MapLogic* map_logic, class UnitNode* unit_node ) {
    ValueVector& trigger_vector = _logic_event.at( "triggers" ).asValueVector();
    int count = (int)trigger_vector.size();
    if( _relation == "then" ) {
        count = 1;
    }
    for( int i = 0; i < count; i++ ) {
        const ValueMap& trigger_data_map = trigger_vector.at( i ).asValueMap();
        bool is_qualified = true;
        for( auto pair : conditions ) {
            std::string key = pair.first;
            auto itr = trigger_data_map.find( key );
            if( itr == trigger_data_map.end() || itr->second != pair.second ) {
                is_qualified = false;
                break;
            }
        }
        if( is_qualified ) {
            this->onSingleTriggerPass( i, map_logic, unit_node );
        }
    }
}

void EventTrigger::activateTriggerByUnit( class MapLogic* map_logic, class UnitNode* unit, const std::string& unit_state, const cocos2d::ValueMap& area ) {
    eUnitCamp camp = unit->getUnitCamp();
    ValueVector& trigger_vector = _logic_event.at( "triggers" ).asValueVector();
    int count = (int)trigger_vector.size();
    if( _relation == "then" ) {
        count = 1;
    }
    for( int i = 0; i < count; i++ ) {
        const ValueMap& trigger_data_map = trigger_vector.at( i ).asValueMap();
        std::string trigger_type = trigger_data_map.at( "trigger_type" ).asString();
        std::string unit_state = "";
        auto itr = trigger_data_map.find( "unit_state" );
        if( itr != trigger_data_map.end() ) {
            unit_state = itr->second.asString();
        }
        
        if( trigger_type == EVENT_TRIGGER_TYPE_UNIT_CHANGE && unit_state == unit_state ) {
            std::string source_type = trigger_data_map.at( "source_type" ).asString();
            std::string source_value = trigger_data_map.at( "source_value" ).asString();
            std::string position_name = trigger_data_map.at( "position_name" ).asString();
            int trigger_count = 1;
            itr = trigger_data_map.find( "trigger_count" );
            if( itr != trigger_data_map.end() ) {
                trigger_count = itr->second.asInt();
            }
            
            if( source_type == UNIT_SOURCE_TYPE && camp == UnitNode::getCampByString( source_value ) ) {
                if( unit_state == UNIT_STATE_APPEAR ) {
                    if( trigger_count == 1 || trigger_count == map_logic->getUnitAppearCountByCamp( source_value ) ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
                else if( unit_state == UNIT_STATE_DISAPPEAR ) {
                    if( trigger_count == 1 || trigger_count == map_logic->getUnitDisappearCountByCamp( source_value ) ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( trigger_count == 0 ) {
                        std::list<UnitNode*> units = unit->getBattleLayer()->getAliveUnitsByCamp( UnitNode::getCampByString( source_value ) );
                        if( units.size() == 0 ) {
                            this->onSingleTriggerPass( i, map_logic, unit );
                        }
                    }
                }
                else if( unit_state == UNIT_STATE_MOVE_TO ) {
                    MapData* map_data = unit->getBattleLayer()->getMapData();
                    int in_area_count = 0;
                    std::list<UnitNode*> units = unit->getBattleLayer()->getAliveUnitsByCamp( UnitNode::getCampByString( source_value ) );
                    if( units.size() == 0 ) {
                        continue;
                    }
                    for( auto u : units ) {
                        ValueMap unit_area = map_data->getAreaMapByPosition( u->getPosition() );
                        if( ( unit_area.find( "name" ) != unit_area.end() ) && ( unit_area.at( "name" ).asString() == position_name ) ) {
                            ++in_area_count;
                        }
                    }
                    if( trigger_count == 0 && in_area_count == units.size() ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( trigger_count == 1 && area.at( "name" ).asString() == position_name ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( in_area_count == trigger_count ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
            }
            else if( source_type == UNIT_SOURCE_TAG && unit->hasUnitTag( source_value ) ) {
                if( unit_state == UNIT_STATE_APPEAR ) {
                    if( trigger_count == 0 ) {
                        continue;
                    }
                    else if( trigger_count == 1 || map_logic->getUnitAppearCountByTag( source_value ) == trigger_count ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
                else if( unit_state == UNIT_STATE_DISAPPEAR ) {
                    if( trigger_count == 0 ) {
                        std::list<UnitNode*> units = map_logic->getBattleLayer()->getAliveUnitsByTag( source_value );
                        if( units.size() == 0 ) {
                            this->onSingleTriggerPass( i, map_logic, unit );
                        }
                    }
                    else if( trigger_count == 1 || trigger_count == map_logic->getUnitDisappearCountByTag( source_value ) ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
                else if( unit_state == UNIT_STATE_MOVE_TO ) {
                    MapData* map_data = unit->getBattleLayer()->getMapData();
                    int in_area_count = 0;
                    std::list<UnitNode*> units = map_logic->getBattleLayer()->getAliveUnitsByTag( source_value );
                    if( units.size() == 0 ) {
                        continue;
                    }
                    for( auto u : units ) {
                        ValueMap unit_area = map_data->getAreaMapByPosition( u->getPosition() );
                        if( ( unit_area.find( "name" ) != unit_area.end() ) && ( unit_area.at( "name" ).asString() == position_name ) ) {
                            ++in_area_count;
                        }
                    }
                    if( trigger_count == 0 && in_area_count == units.size() ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( trigger_count == 1 && area.at( "name" ).asString() == position_name ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( in_area_count == trigger_count ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
            }
            else if( source_type == UNIT_SOURCE_NAME && unit->getUnitData()->name == source_value ) {
                if( unit_state == UNIT_STATE_APPEAR ) {
                    if( trigger_count == 0 ) {
                        continue;
                    }
                    else if( trigger_count == 1 || map_logic->getUnitAppearCountByName( source_value ) ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
                else if( unit_state == UNIT_STATE_DISAPPEAR ) {
                    if( trigger_count == 0 ) {
                        std::list<UnitNode*> units = map_logic->getBattleLayer()->getAliveUnitsByName( source_value );
                        if( units.size() == 0 ) {
                            this->onSingleTriggerPass( i, map_logic, unit );
                        }
                    }
                    else if( trigger_count == 1 || trigger_count == map_logic->getUnitDisappearCountByName( source_value ) ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
                else if( unit_state == UNIT_STATE_MOVE_TO ) {
                    MapData* map_data = unit->getBattleLayer()->getMapData();
                    int in_area_count = 0;
                    std::list<UnitNode*> units = map_logic->getBattleLayer()->getAliveUnitsByName( source_value );
                    if( units.size() == 0 ) {
                        continue;
                    }
                    for( auto u : units ) {
                        ValueMap unit_area = map_data->getAreaMapByPosition( u->getPosition() );
                        if( ( unit_area.find( "name" ) != unit_area.end() ) && ( unit_area.at( "name" ).asString() == position_name ) ) {
                            ++in_area_count;
                        }
                    }
                    if( trigger_count == 0 && in_area_count == units.size() ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( trigger_count == 1 && area.at( "name" ).asString() == position_name ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                    else if( in_area_count == trigger_count ) {
                        this->onSingleTriggerPass( i, map_logic, unit );
                    }
                }
            }
        }
    }
}