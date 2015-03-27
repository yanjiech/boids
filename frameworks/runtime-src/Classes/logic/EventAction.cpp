//
//  EventAction.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#include "EventAction.h"
#include "../util/CocosUtils.h"
#include "EventTrigger.h"
#include "MapLogic.h"
#include "../unit/UnitNode.h"

using namespace cocos2d;

EventAction::EventAction() {
    
}

EventAction::~EventAction() {
    
}

EventAction* EventAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger   ) {
    EventAction* ret = new EventAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EventAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger  ) {
    _action_data = action_data;
    _map_logic = map_logic;
    _trigger = trigger;
    std::string meta = _action_data.at( "meta" ).asString();
    std::vector<std::string> str_vector;
    Utils::split( meta, str_vector, ',' );
    _repeat = Utils::toInt( str_vector[0] );
    _delay = (float)Utils::toDouble( str_vector[1] );
    if( _delay == 0 ) {
        _delay = 0.01;
    }
    _interval = (float)Utils::toDouble( str_vector[2] );
    
    _auto_unschedule = false;
    _is_running = false;
    _accumulator = 0;
    _current_round = 0;
    _should_recycle = false;
    
    _action_name = CocosUtils::getNextGlobalBoidsEventActionId();
    return true;
}

bool EventAction::start( cocos2d::Map<std::string, cocos2d::Ref*> params, bool auto_unschedule ) {
    if( !_is_running ) {
        _params = params;
        _callback = CC_CALLBACK_1( EventAction::onEventTriggered, this );
        _auto_unschedule = auto_unschedule;
        _is_running = true;
        return true;
    }
    return false;
}

void EventAction::stop() {
    if( _is_running ) {
        _should_recycle = true;
        _is_running = false;
    }
}

void EventAction::updateFrame( float delta ) {
    if( _is_running ) {
        _accumulator += delta;
        if( ( _current_round == 0 && _accumulator > _delay ) || ( _current_round > 0 && _accumulator > _interval ) ) {
            _accumulator = 0;
            bool finish = false;
            if( !this->isInfinite() ) {
                ++_current_round;
                finish = _current_round > _repeat;
            }
            _callback( finish );
            if( finish && _auto_unschedule ) {
                this->stop();
            }
        }
    }
}

void EventAction::onEventTriggered( bool finish ) {
    const ValueMap& event = _trigger->getEventData();
    _map_logic->onEventChanged( event.at( "name" ).asString(), EVENT_STATE_TRIGGER );
    const std::string& action_type = _action_data.at( "action_type" ).asString();
    if( action_type == ACTION_TYPE_UNIT_CHANGE ) {
        UnitNode* unit_node = dynamic_cast<UnitNode*>( _params.at( "unit" ) );
        _map_logic->executeUnitAction( _action_data, unit_node );
    }
    else if( action_type == ACTION_TYPE_TASK_CHANGE ) {
        const std::string& task_name = _action_data.at( "task_name" ).asString();
        const std::string& task_state = _action_data.at( "task_state" ).asString();
        _map_logic->onTaskStateChanged( task_name, task_state );
    }
    else if( action_type == ACTION_TYPE_GAME_CHANGE ) {
        const std::string& game_state = _action_data.at( "game_state" ).asString();
        _map_logic->onGameStateChanged( game_state );
    }
    else if( action_type == ACTION_TYPE_CUSTOM ) {
        _map_logic->executeCustomAction( _action_data );
    }
    else if( action_type == ACTION_TYPE_EVENT_CHANGE ) {
        const std::string& event_name = _action_data.at( "event_name" ).asString();
        bool enabled = _action_data.at( "event_state" ).asString() == EVENT_STATE_ENABLE;
        _map_logic->setTriggersEnabledOfName( event_name, enabled );
    }
    else if( action_type == ACTION_TYPE_VISION_CHANGE ) {
        _map_logic->onVisionChanged( _action_data );
    }
    else if( action_type == ACTION_TYPE_WAVE_ACTION ) {
        Node* node = dynamic_cast<Node*>( _params.at( "idx" ) );
        std::string wave_tag = Utils::stringFormat( "%s_%d", event.at( "name" ).asString().c_str(), node->getTag() );
        _map_logic->executeWaveAction( _action_data, wave_tag );
    }
    else if( action_type == ACTION_TYPE_CONVERSATION_ACTION ) {
        _map_logic->executeConversationAction( _action_data );
    }
}