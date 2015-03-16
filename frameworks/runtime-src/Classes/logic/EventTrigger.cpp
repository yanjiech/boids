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

std::vector<int> EventTrigger::getTriggerIndexesForTest() {
    std::vector<int> ret;
    if( _relation == "then" ) {
        ret.push_back( 0 );
    }
    else {
        for( int i = 0; i < _total_trigger_count; i++ ) {
            ret.push_back( i );
        }
    }
    return ret;
}