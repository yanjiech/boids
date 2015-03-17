//
//  MapLogic.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "MapLogic.h"
#include "../scene/BattleLayer.h"
#include "../data/PlayerInfo.h"
#include "../behavior/BehaviorHeader.h"
#include "../util/CocosUtils.h"
#include "../constant/BoidsConstant.h"

using namespace cocos2d;

GameTask::GameTask( const std::string& name, const std::string& desc, const std::string& state ) {
    this->task_name = name;
    this->task_desc = desc;
    this->task_state = state;
}

GameTask::GameTask( const GameTask& other ) {
    this->task_name = other.task_name;
    this->task_desc = other.task_desc;
    this->task_state = other.task_state;
}

GameTask& GameTask::operator=( const GameTask& other ) {
    this->task_name = other.task_name;
    this->task_desc = other.task_desc;
    this->task_state = other.task_state;
    return *this;
}

MapLogic::MapLogic() {
    
}

MapLogic::~MapLogic() {
    
}

MapLogic* MapLogic::retainedCreate( BattleLayer* battle_layer ) {
    MapLogic* ret = new MapLogic();
    if( ret && ret->init( battle_layer ) ) {
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MapLogic::init( BattleLayer* battle_layer ) {
    _battle_layer = battle_layer;
    
    const rapidjson::Document& meta_json = _battle_layer->getMapData()->getMetaJson();
    
    const rapidjson::Value& task_json = meta_json["tasks"];
    for( rapidjson::Value::ConstValueIterator itr = task_json.onBegin(); itr != task_json.onEnd(); ++itr ) {
        const rapidjson::Value& t = *itr;
        std::string name = std::string( t["name"].GetString(), t["name"].GetStringLength() );
        std::string desc = std::string( t["desc"].GetString(), t["desc"].GetStringLength() );
        GameTask gt = GameTask( name, desc, GAME_TASK_STATE_UNSTARTED );
        _game_tasks.insert( std::make_pair( name, gt ) );
    }
    
    const rapidjson::Value& events_json = meta_json["events"];
    for( rapidjson::Value::ConstValueIterator itr = events_json.onBegin(); itr != events_json.onEnd(); ++itr ) {
        EventTrigger* trigger = EventTrigger::create( *itr );
        _triggers.pushBack( trigger );
    }
    
    if( meta_json.HasMember( "conditions" ) ) {
        _conditions = CocosUtils::jsonArrayToValueVector( meta_json["conditions"] );
    }
    
    this->deployPlayerUnits();
    
    return true;
}

void MapLogic::updateFrame( float delta ) {
    this->updateEventActions( delta );
}

void MapLogic::deployPlayerUnits() {
    const rapidjson::Value& region = _battle_layer->getMapData()->getAreaByName( "player_start" );
    
    float x = region["rect"]["x"].GetDouble();
    float y = region["rect"]["y"].GetDouble();
    float width = region["rect"]["width"].GetDouble();
    float height = region["rect"]["height"].GetDouble();
    
    cocos2d::Rect player_start_area( x, y, width, height );
    
    const rapidjson::Value& player_units = PlayerInfo::getInstance()->getPlayerUnitsInfo();
    for( rapidjson::Value::ConstValueIterator itr = player_units.onBegin(); itr != player_units.onEnd(); ++itr ) {
        const rapidjson::Value& value = *itr;
        ValueMap vm;
        vm["name"] = Value( value["name"].GetString() );
        vm["level"] = Value( value["level"].GetInt() );
        UnitNode* unit = UnitNode::create( _battle_layer, vm );
        unit->setUnitCamp( eUnitCamp::Player );
        
        PlayerMoveBehavior* move_behavior = PlayerMoveBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_MOVE, move_behavior );
        IdleBehavior* idle_behavior = IdleBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_IDLE, idle_behavior );
        PlayerAttackBehavior* attack_behavior = PlayerAttackBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
        PlayerSkillBehavior* skill_behavior = PlayerSkillBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_SKILL, skill_behavior );
        
        cocos2d::Point pos = _battle_layer->getAvailablePosition( unit->getUnitData()->collide, player_start_area );
        _battle_layer->deployUnit( unit, pos, "player" );
    }
}

void MapLogic::executeLogicEvent( EventTrigger* trigger, UnitNode* unit_node ) {
    const cocos2d::ValueMap& event = trigger->getLogicEvent();
    const cocos2d::ValueVector& actions = event.at( "actions" ).asValueVector();
    int action_count = actions.size();
    for( int i = 0; i < action_count; ++i ) {
        const ValueMap& action = actions.at( i ).asValueMap();
        EventAction* ea = EventAction::create( action, this, trigger );
        this->addEventAction( ea, ea->getActionName() );
        cocos2d::Map<std::string, cocos2d::Ref*> params;
        if( unit_node ) {
            params.insert( "unit", unit_node );
        }
        else {
            params.insert( "unit", Node::create() );
        }
        Node* node = Node::create();
        node->setTag( i );
        params.insert( "idx", node );
        ea->start( params, true );
    }
}

void MapLogic::executeUnitAction( const cocos2d::ValueMap& action_data, UnitNode* unit_node ) {
    std::list<UnitNode*> source_units;
    bool should_perform_changes = false;
    std::string source_type = action_data.at( "source_type" ).asString();
    std::string source_value = action_data.at( "source_value" ).asString();
    std::string unit_state = "";
    auto itr = action_data.find( "unit_state" );
    if( itr != action_data.end() ) {
        unit_state = itr->second.asString();
    }
    
    bool show_hp = false;
    itr = action_data.find( "show_hp" );
    if( itr != action_data.end() ) {
        show_hp = itr->second.asBool();
    }
    
    std::string unit_type = "";
    itr = action_data.find( "unit_type" );
    if( itr != action_data.end() ) {
        unit_type = itr->second.asString();
    }
    
    std::string buff_name = "";
    itr = action_data.find( "buff_name" );
    if( itr != action_data.end() ) {
        buff_name = itr->second.asString();
    }
    
    std::string tag_name = "";
    itr = action_data.find( "tag_name" );
    if( itr != action_data.end() ) {
        tag_name = itr->second.asString();
    }
    
    std::vector<std::string> buffs_to_add;
    std::vector<std::string> buffs_to_remove;
    itr = action_data.find( "buff_name" );
    if( itr != action_data.end() ) {
        std::vector<std::string> buff_names;
        std::string buff_names_string = itr->second.asString();
        Utils::split( buff_names_string, buff_names, ',' );
        for( auto str : buff_names ) {
            std::string buff_name = str.substr( 2, str.length() );
            std::string opt = str.substr( 1, 1 );
            if( opt == "+" ) {
                buffs_to_add.push_back( buff_name );
            }
            else if( opt == "-" ) {
                buffs_to_remove.push_back( buff_name );
            }
        }
    }
    if( source_type == UNIT_SOURCE_TYPE ) {
        source_units = _battle_layer->getAliveUnitsByCamp( UnitNode::getCampByString( source_value ) );
    }
    else if( source_type == UNIT_SOURCE_TRIGGERED ) {
        if( unit_node ) {
            source_units.push_back( unit_node );
        }
    }
    else if( source_type == UNIT_SOURCE_TAG ) {
        source_units = _battle_layer->getAliveUnitsByTag( source_value );
    }
    else if( source_type == UNIT_SOURCE_NAME ) {
        if( unit_state == UNIT_STATE_APPEAR || unit_state == UNIT_STATE_HOLD_APPEAR ) {
            should_perform_changes = false;
            cocos2d::ValueVector areas;
            auto itr = action_data.find( "position_name" );
            if( itr != action_data.end() ) {
                areas.push_back( Value( _battle_layer->getMapData()->getAreaMapByName( itr->second.asString() ) ) );
            }
            else {
                areas = _battle_layer->getMapData()->getAreasVectorByTag( action_data.at( "position_name" ).asString() );
            }
            
            for( auto ar : areas ) {
                const ValueMap& ar_data = ar.asValueMap();
                const ValueMap& rect_data = ar_data.at( "rect" ).asValueMap();
                float x = rect_data.at( "x" ).asFloat();
                float y = rect_data.at( "y" ).asFloat();
                float width = rect_data.at( "width" ).asFloat();
                float height = rect_data.at( "height" ).asFloat();
                Rect deploy_rect = Rect( x, y, width, height );
                if( unit_type == "item" ) {
                    //todo
                }
                else {
                    int unit_count = 1;
                    itr = action_data.find( "unit_count" );
                    if( itr != action_data.end() ) {
                        unit_count = itr->second.asInt();
                    }
                    int unit_level = 1;
                    itr = action_data.find( "unit_level" );
                    if( itr != action_data.end() ) {
                        unit_level = itr->second.asInt();
                    }
                    itr = action_data.find( "is_boss" );
                    bool is_boss = false;
                    if( itr != action_data.end() ) {
                        is_boss = itr->second.asBool();
                    }
                    bool hold_position = ( unit_state == UNIT_STATE_HOLD_APPEAR );
                    std::string custom_change = "";
                    itr = action_data.find( "custom_change" );
                    if( itr != action_data.end() ) {
                        custom_change = itr->second.asString();
                    }
                    
                    std::list<UnitNode*> deploy_units;
                    for( int i = 0; i < unit_count; ++i ) {
                        ValueMap unit_data;
                        unit_data["name"] = Value( source_value );
                        unit_data["level"] = Value( unit_level );
                        unit_data["show_hp"] = Value( show_hp );
                        unit_data["is_boss"] = Value( is_boss );
                        unit_data["hold_position"] = Value( hold_position );
                        unit_data["unit_camp"] = Value( unit_type );
                        unit_data["tag_string"] = Value( tag_name );
                        
                        UnitNode* unit = UnitNode::create( _battle_layer, unit_data );
                        deploy_units.push_back( unit );
                        
                        AttackBehavior* attack_behavior = AttackBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
                        MoveBehavior* move_behavior = MoveBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_MOVE, move_behavior );
                        IdleBehavior* idle_behavior = IdleBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_IDLE, idle_behavior );
                        
                        if( custom_change != "" ) {
                            unit->applyCustomChange( custom_change );
                        }
                    }
                    
                    std::string sight_group;
                    itr = action_data.find( "sight_group" );
                    if( itr != action_data.end() ) {
                        sight_group = itr->second.asString();
                    }
                    else {
                        sight_group = ar_data.at( "name" ).asString().c_str() + action_data.at( "meta" ).asString();
                    }
                    _battle_layer->deployUnits( deploy_units, deploy_rect, sight_group );
                }
            }
        }
    }
    
    if( should_perform_changes ) {
        for( auto u : source_units ) {
            if( unit_state == UNIT_STATE_MOVE_TO ) {
                
            }
            else if( unit_state == UNIT_STATE_PATROL ) {
                
            }
            else {
                
            }
            if( show_hp ) {
                u->showHP();
            }
            else {
                u->hideHP();
            }
            if( tag_name != "" ) {
                u->setUnitTags( tag_name );
            }
            if( unit_type != "" ) {
                u->setUnitCamp( UnitNode::getCampByString( unit_type ) );
            }
        }
    }
}

void MapLogic::executeWaveAction( const cocos2d::ValueMap& action_data, const std::string& wave_action_tag ) {
    
}

void MapLogic::executeConversationAction( const cocos2d::ValueMap& action_data ) {
    
}

void MapLogic::executeSpeech( const cocos2d::ValueMap& action_data ) {
    
}

void MapLogic::executeCustomAction( const cocos2d::ValueMap& action_data ) {
    
}

void MapLogic::onEventChanged( const std::string& event_name, const std::string& event_state ) {
    for( auto trigger : _triggers ) {
        if( trigger->isValid() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_EVENT_CHANGE );
            conditions["event_name"] = Value( event_name );
            conditions["event_state"] = Value( event_state );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::onMapInit() {
    for( auto trigger : _triggers ) {
        if( trigger->isValid() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_MAP_INIT );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::onTaskStateChanged( const std::string& task_name, const std::string& task_state ) {
    
}

void MapLogic::onGameStateChanged( const std::string& game_state ) {
    //todo
    
    //triggers
    for( auto trigger : _triggers ) {
        if( trigger->isValid() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_GAME_CHANGE );
            conditions["game_state"] = Value( game_state );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::onVisionChanged( const cocos2d::ValueMap& action_data ) {
    
}

void MapLogic::onCustomTrigger( const std::string& trigger_name ) {
    for( auto trigger : _triggers ) {
        if( trigger->isValid() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_CUSTOM );
            conditions["trigger_name"] = Value( trigger_name );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::onConversationStateChanged( const std::string& trigger_name, const std::string& trigger_state ) {
    for( auto trigger : _triggers ) {
        if( trigger->isValid() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_CONVERSATION_CHANGE );
            conditions["name"] = Value( trigger_name );
            conditions["state"] = Value( trigger_state );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::addEventAction( EventAction* action, const std::string& key ) {
    _event_actions.insert( key, action );
}

void MapLogic::removeEventAction( const std::string& key ) {
    _event_actions.erase( key );
}

void MapLogic::updateEventActions( float delta ) {
    auto itr = _event_actions.begin();
    while( itr != _event_actions.end() ) {
        if( itr->second->shouldRecycle() ) {
            itr = _event_actions.erase( itr );
        }
        else {
            itr->second->updateFrame( delta );
            ++itr;
        }
    }
}

void MapLogic::setTriggersEnabledOfName( const std::string& name, bool b ) {
    for( auto trigger : _triggers ) {
        if( trigger->getLogicEvent().at( "name" ).asString() == name ) {
            trigger->setEnabled( b );
        }
    }
}

void MapLogic::checkGameState() {
    
}

void MapLogic::onTargetNodeAppear( class TargetNode* target_node ) {
    
}

void MapLogic::onTargetNodeDisappear( class TargetNode* target_node ) {
    
}

int MapLogic::getUnitAppearCountByCamp( const std::string& camp ) {
    return _unit_appear_count_by_camp[camp].asInt();
}

int MapLogic::getUnitDisappearCountByCamp( const std::string& camp ) {
    return _unit_disappear_count_by_camp[camp].asInt();
}

int MapLogic::getUnitAppearCountByTag( const std::string& tag ) {
    return _unit_appear_count_by_tag[tag].asInt();
}

int MapLogic::getUnitDisappearCountByTag( const std::string& tag ) {
    return _unit_disappear_count_by_tag[tag].asInt();
}

int MapLogic::getUnitAppearCountByName( const std::string& name ) {
    return _unit_appear_count_by_name[name].asInt();
}

int MapLogic::getUnitDisappearCountByName( const std::string& name ) {
    return _unit_disappear_count_by_name[name].asInt();
}