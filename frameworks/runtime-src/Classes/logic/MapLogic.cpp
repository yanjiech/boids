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
#include "MapLogic.h"

using namespace cocos2d;

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
    
    const ValueMap& meta_json = _battle_layer->getMapData()->getMetaJson();
    
    auto sitr = meta_json.find( "conditions" );
    if( sitr != meta_json.end() ) {
        const ValueVector& task_json = sitr->second.asValueVector();
        for( auto itr = task_json.begin(); itr != task_json.end(); ++itr ) {
            const ValueMap& t = itr->asValueMap();
            GameTask* gt = GameTask::create( t, this );
//            _game_tasks.pushBack( gt );
        }
    }
    
    const ValueVector& events_json = meta_json.at( "events" ).asValueVector();
    for( auto itr = events_json.begin(); itr != events_json.end(); ++itr ) {
        EventTrigger* trigger = EventTrigger::create( itr->asValueMap() );
        _triggers.pushBack( trigger );
    }
    
    return true;
}

void MapLogic::updateFrame( float delta ) {
    this->updateEventActions( delta );
    this->checkTriggers();
    this->checkGameState( delta );
}

void MapLogic::gameEndWithResult( const cocos2d::ValueMap& result ) {
    
}

void MapLogic::deployPlayerUnits() {
    ValueMap region = _battle_layer->getMapData()->getAreaMapByName( "player_start" );
    const ValueMap& rect_data = region.at( "rect" ).asValueMap();
    
    float x = rect_data.at( "x" ).asFloat();
    float y = rect_data.at( "y" ).asFloat();
    float width = rect_data.at( "width" ).asFloat();
    float height = rect_data.at( "height" ).asFloat();
    
    cocos2d::Rect player_start_area( x, y, width, height );
    
    const ValueVector& player_units = PlayerInfo::getInstance()->getPlayerDeployedUnitsInfo();
    for( auto itr = player_units.begin(); itr != player_units.end(); ++itr ) {
        const ValueMap& vm = itr->asValueMap();
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
    cocos2d::Vector<UnitNode*> source_units;
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
            source_units.pushBack( unit_node );
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
                    
                    cocos2d::Vector<UnitNode*> deploy_units;
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
                        deploy_units.pushBack( unit );
                        
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
    this->deployPlayerUnits();
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

void MapLogic::checkTriggers() {
    
}

void MapLogic::checkGameState( float delta ) {
    ValueMap result;
    for( auto itr = _game_tasks.begin(); itr != _game_tasks.end(); ++itr ) {
        GameTask* task = *itr;
        task->updateFrame( delta );
        if( task->isPrimary() ) {
            if( task->getTaskState() == GAME_TASK_STATE_FINISHED ) {
                //win
                result["result"] = Value( "win" );
                this->gameEndWithResult( result );
            }
            else if( task->getTaskState() == GAME_TASK_STATE_FAILED ) {
                //lose
                result["result"] = Value( "lose" );
                this->gameEndWithResult( result );
            }
        }
    }
}

void MapLogic::onTargetNodeAppear( TargetNode* target_node ) {
    do {
        UnitNode* unit_node = dynamic_cast<UnitNode*>( target_node );
        if( unit_node ) {
            this->increaseUnitAppearCountByCamp( 1, (int)unit_node->getUnitCamp() );
            cocos2d::ValueVector tags = unit_node->getUnitTags();
            for( auto itr = tags.begin(); itr != tags.end(); ++itr ) {
                this->increaseUnitAppearCountByTag( 1, itr->asString() );
            }
            this->increaseUnitAppearCountByName( 1, unit_node->getUnitData()->name );
            break;
        }
    }while( false );
}

void MapLogic::onTargetNodeDisappear( TargetNode* target_node ) {
    do {
        UnitNode* unit_node = dynamic_cast<UnitNode*>( target_node );
        if( unit_node ) {
            this->increaseUnitDisappearCountByCamp( 1, (int)unit_node->getUnitCamp() );
            cocos2d::ValueVector tags = unit_node->getUnitTags();
            for( auto itr = tags.begin(); itr != tags.end(); ++itr ) {
                this->increaseUnitDisappearCountByTag( 1, itr->asString() );
            }
            this->increaseUnitDisappearCountByName( 1, unit_node->getUnitData()->name );
            break;
        }
    }while( false );
}

int MapLogic::getUnitAppearCountByCamp( int camp ) {
    auto itr = _unit_appear_count_by_camp.find( camp );
    if( itr != _unit_appear_count_by_camp.end() ) {
        return itr->second.asInt();
    }
    return 0;
}

void MapLogic::increaseUnitAppearCountByCamp( int count, int camp ) {
    auto itr = _unit_appear_count_by_camp.find( camp );
    if( itr != _unit_appear_count_by_camp.end() ) {
        itr->second = Value( itr->second.asInt() + count );
    }
    else {
        _unit_appear_count_by_camp[camp] = Value( count );
    }
}

int MapLogic::getUnitDisappearCountByCamp( int camp ) {
    auto itr = _unit_disappear_count_by_camp.find( camp );
    if( itr != _unit_disappear_count_by_camp.end() ) {
        return itr->second.asInt();
    }
    return 0;
}

void MapLogic::increaseUnitDisappearCountByCamp( int count, int camp ) {
    auto itr = _unit_disappear_count_by_camp.find( camp );
    if( itr != _unit_disappear_count_by_camp.end() ) {
        itr->second = Value( itr->second.asInt() + count );
    }
    else {
        _unit_disappear_count_by_camp[camp] = Value( count );
    }
}

int MapLogic::getUnitAppearCountByTag( const std::string& tag ) {
    auto itr = _unit_appear_count_by_tag.find( tag );
    if( itr != _unit_appear_count_by_tag.end() ) {
        return itr->second.asInt();
    }
    return 0;
}

void MapLogic::increaseUnitAppearCountByTag( int count, const std::string& tag ) {
    auto itr = _unit_appear_count_by_tag.find( tag );
    if( itr != _unit_appear_count_by_tag.end() ) {
        itr->second = Value( itr->second.asInt() + count );
    }
    else {
        _unit_appear_count_by_tag[tag] = Value( count );
    }
}

int MapLogic::getUnitDisappearCountByTag( const std::string& tag ) {
    auto itr = _unit_disappear_count_by_tag.find( tag );
    if( itr != _unit_disappear_count_by_tag.end() ) {
        return itr->second.asInt();
    }
    return 0;
}

void MapLogic::increaseUnitDisappearCountByTag( int count, const std::string& tag ) {
    auto itr = _unit_disappear_count_by_tag.find( tag );
    if( itr != _unit_disappear_count_by_tag.end() ) {
        itr->second = Value( itr->second.asInt() + count );
    }
    else {
        _unit_disappear_count_by_tag[tag] = Value( count );
    }
}

int MapLogic::getUnitAppearCountByName( const std::string& name ) {
    auto itr = _unit_appear_count_by_name.find( name );
    if( itr != _unit_appear_count_by_name.end() ) {
        return itr->second.asInt();
    }
    return 0;
}

void MapLogic::increaseUnitAppearCountByName( int count, const std::string& name ) {
    auto itr = _unit_appear_count_by_name.find( name );
    if( itr != _unit_appear_count_by_name.end() ) {
        itr->second = Value( itr->second.asInt() + count );
    }
    else {
        _unit_appear_count_by_name[name] = Value( count );
    }
}

int MapLogic::getUnitDisappearCountByName( const std::string& name ) {
    auto itr = _unit_disappear_count_by_name.find( name );
    if( itr != _unit_disappear_count_by_name.end() ) {
        return itr->second.asInt();
    }
    return 0;
}

void MapLogic::increaseUnitDisappearCountByName( int count, const std::string& name ) {
    auto itr = _unit_disappear_count_by_name.find( name );
    if( itr != _unit_disappear_count_by_name.end() ) {
        itr->second = Value( itr->second.asInt() + count );
    }
    else {
        _unit_disappear_count_by_name[name] = Value( count );
    }
}