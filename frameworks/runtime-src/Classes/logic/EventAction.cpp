//
//  EventAction.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#include "EventAction.h"
#include "../util/CocosUtils.h"
#include "../scene/BattleLayer.h"
#include "../AI/Path.h"

using namespace cocos2d;

EventAction::EventAction() :
_callback( nullptr )
{
    
}

EventAction::~EventAction() {
    
}

EventAction* EventAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger   ) {
    EventAction* ret = nullptr;
    std::string action_type = action_data.at( "action_type" ).asString();
    if( action_type == ACTION_TYPE_UNIT_CHANGE ) {
        ret = UnitChangeAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_TASK_CHANGE ) {
        ret = TaskChangeAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_GAME_CHANGE ) {
        ret = GameChangeAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_EVENT_CHANGE ) {
        ret = EventChangeAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_VISION_CHANGE ) {
        ret = VisionChangeAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_WAVE_ACTION ) {
        ret = WaveAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_CONVERSATION_ACTION ) {
        ret = ConversationAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_STORY_ACTION ) {
        ret = StoryAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_CUSTOM ) {
        ret = CustomAction::create( action_data, map_logic, trigger );
    }
    else if( action_type == ACTION_TYPE_HINT_CHANGE ) {
        ret = HintChangeAction::create( action_data, map_logic, trigger );
    }
    return ret;
}

bool EventAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger  ) {
    _action_data = action_data;
    _map_logic = map_logic;
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
    
    this->setTriggerName( trigger->getEventTriggerName() );
    return true;
}

bool EventAction::start( const cocos2d::Map<std::string, cocos2d::Ref*>& params, bool auto_unschedule ) {
    if( !_is_running ) {
        _params = params;
        _auto_unschedule = auto_unschedule;
        _is_running = true;
        return true;
    }
    return false;
}

void EventAction::pause() {
    _is_running = false;
}

void EventAction::resume() {
    _is_running = true;
}

void EventAction::stop() {
    if( _is_running ) {
        _should_recycle = true;
        _is_running = false;
    }
}

void EventAction::updateFrame( float delta ) {
    if( !_should_recycle && _is_running ) {
        _accumulator += delta;
        if( ( _current_round == 0 && _accumulator > _delay ) || ( _current_round > 0 && _accumulator > _interval ) ) {
            _accumulator = 0;
            bool finish = false;
            ++_current_round;
            if( !this->isInfinite() ) {
                finish = _current_round > _repeat;
            }
            if( _callback ) {
                _callback( finish );
            }
            if( finish && _auto_unschedule ) {
                this->stop();
            }
        }
    }
}

void EventAction::onActionTriggered( bool finish ) {
}

//unit change aciton
UnitChangeAction::UnitChangeAction() {
    
}

UnitChangeAction::~UnitChangeAction() {
    
}

UnitChangeAction* UnitChangeAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    UnitChangeAction* ret = new UnitChangeAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitChangeAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( UnitChangeAction::onActionTriggered, this );
    return true;
}

void UnitChangeAction::onActionTriggered( bool finish ) {
    EventAction::onActionTriggered( finish );
    
    UnitNode* unit_node = dynamic_cast<UnitNode*>( _params.at( "unit" ) );
    BattleLayer* battle_layer = _map_logic->getBattleLayer();
    
    cocos2d::Vector<UnitNode*> source_units;
    bool should_perform_changes = true;
    std::string source_type = _action_data.at( "source_type" ).asString();
    std::string source_value = _action_data.at( "source_value" ).asString();
    std::string unit_state = "";
    auto itr = _action_data.find( "unit_state" );
    if( itr != _action_data.end() ) {
        unit_state = itr->second.asString();
    }
    
    bool change_show_hp;
    bool show_hp = false;
    itr = _action_data.find( "show_hp" );
    if( itr != _action_data.end() ) {
        show_hp = itr->second.asBool();
        change_show_hp = true;
    }
    else {
        change_show_hp = false;
    }
    
    std::string unit_type = "";
    itr = _action_data.find( "unit_type" );
    if( itr != _action_data.end() ) {
        unit_type = itr->second.asString();
    }
    
    std::string buff_name = "";
    itr = _action_data.find( "buff_name" );
    if( itr != _action_data.end() ) {
        buff_name = itr->second.asString();
    }
    
    std::string tag_name = "";
    itr = _action_data.find( "tag_name" );
    if( itr != _action_data.end() ) {
        tag_name = itr->second.asString();
    }
    
    bool get_or_lose_item = true;
    itr = _action_data.find( "item_get_or_lose" );
    if( itr != _action_data.end() ) {
        get_or_lose_item = itr->second.asBool();
    }
    std::string item_name = "";
    itr = _action_data.find( "item_name" );
    if( itr != _action_data.end() ) {
        item_name = itr->second.asString();
    }
    std::string item_resource = "";
    itr = _action_data.find( "item_resource" );
    if( itr != _action_data.end() ) {
        item_resource = itr->second.asString();
    }
    
    ValueMap buff_data;
    itr = _action_data.find( "buff_name" );
    if( itr != _action_data.end() ) {
        buff_data["buff_name"] = itr->second;
        buff_data["buff_type"] = _action_data.at( "buff_type" );
        std::string buff_params = _action_data.at( "buff_params" ).asString();
        
        std::vector<std::string> buff_param_pairs;
        Utils::split( buff_params, buff_param_pairs, ',' );
        
        for( auto pair_str : buff_param_pairs ) {
            std::vector<std::string> pair;
            Utils::split( pair_str, pair, ':' );
            buff_data[pair[0]] = Value( pair[1] );
        }
    }
    if( source_type == UNIT_SOURCE_TYPE ) {
        source_units = battle_layer->getAliveUnitsByCamp( UnitNode::getCampByString( source_value ) );
    }
    else if( source_type == UNIT_SOURCE_TRIGGERED ) {
        if( unit_node ) {
            source_units.pushBack( unit_node );
        }
    }
    else if( source_type == UNIT_SOURCE_TAG ) {
        source_units = battle_layer->getAliveUnitsByTag( source_value );
    }
    else if( source_type == UNIT_SOURCE_NAME ) {
        if( unit_state == UNIT_STATE_APPEAR || unit_state == UNIT_STATE_HOLD_APPEAR ) {
            should_perform_changes = false;
            cocos2d::ValueVector areas;
            auto itr = _action_data.find( "position_name" );
            if( itr != _action_data.end() ) {
                areas.push_back( Value( battle_layer->getMapData()->getAreaMapByName( itr->second.asString() ) ) );
            }
            else {
                areas = battle_layer->getMapData()->getAreasVectorByTag( _action_data.at( "position_tag" ).asString() );
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
                    itr = _action_data.find( "unit_count" );
                    if( itr != _action_data.end() ) {
                        unit_count = itr->second.asInt();
                    }
                    int unit_level = 1;
                    itr = _action_data.find( "unit_level" );
                    if( itr != _action_data.end() ) {
                        unit_level = itr->second.asInt();
                    }
                    itr = _action_data.find( "is_boss" );
                    bool is_boss = false;
                    if( itr != _action_data.end() ) {
                        is_boss = itr->second.asBool();
                    }
                    bool hold_position = ( unit_state == UNIT_STATE_HOLD_APPEAR );
                    std::string custom_change = "";
                    itr = _action_data.find( "custom_change" );
                    if( itr != _action_data.end() ) {
                        custom_change = itr->second.asString();
                    }
                    
                    int skill_1_level = 0;
                    itr = _action_data.find( "skill_1_level" );
                    if( itr != _action_data.end() ) {
                        skill_1_level = itr->second.asInt();
                    }
                    
                    int skill_2_level = 0;
                    itr = _action_data.find( "skill_2_level" );
                    if( itr != _action_data.end() ) {
                        skill_2_level = itr->second.asInt();
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
                        ValueVector skills;
                        if( skill_1_level != 0 ) {
                            ValueMap skill_data;
                            skill_data["skill_id"] = Value( 0 );
                            skill_data["level"] = Value( skill_1_level );
                            skills.push_back( Value( skill_data ) );
                        }
                        if( skill_2_level != 0 ) {
                            ValueMap skill_data;
                            skill_data["skill_id"] = Value( 1 );
                            skill_data["level"] = Value( skill_2_level );
                            skills.push_back( Value( skill_data ) );
                        }
                        unit_data["skills"] = Value( skills );
                        
                        UnitNode* unit = UnitNode::create( battle_layer, unit_data );
                        if( !buff_data.empty() ) {
                            Buff* buff = Buff::create( unit, buff_data );
                            unit->addBuff( buff->getBuffId(), buff, true );
                            buff->begin();
                        }
                        
                        deploy_units.pushBack( unit );
                        
                        SkillBehavior* skill_behavior = SkillBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_SKILL, skill_behavior );
                        AttackBehavior* attack_behavior = AttackBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
                        MoveBehavior* move_behavior = MoveBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_MOVE, move_behavior );
                        IdleBehavior* idle_behavior = IdleBehavior::create( unit );
                        unit->addBehavior( BEHAVIOR_NAME_IDLE, idle_behavior );
                        
                        if( custom_change != "" ) {
                            unit->applyCustomChange( custom_change );
                        }
                        
                        if( item_name.size() > 0 ) {
                            if( get_or_lose_item ) {
                                ValueMap item_data;
                                item_data["item_name"] = Value( item_name );
                                item_data["item_resource"] = Value( item_resource );
                                
                                Item* item = Item::create( item_data );
                                unit->addItem( item );
                            }
                            else {
                                unit->removeItem( item_name );
                            }
                        }
                    }
                    
                    std::string sight_group;
                    itr = _action_data.find( "sight_group" );
                    if( itr != _action_data.end() ) {
                        sight_group = itr->second.asString();
                    }
                    else {
                        sight_group = ar_data.at( "name" ).asString().c_str() + _action_data.at( "meta" ).asString();
                    }
                    battle_layer->deployUnits( deploy_units, deploy_rect, sight_group );
                }
            }
        }
    }
    
    if( should_perform_changes ) {
        for( auto u : source_units ) {
            if( unit_state == UNIT_STATE_MOVE_TO || unit_state == UNIT_STATE_PATROL_TO ) {
                Path* path = Path::create( INT_MAX );
                std::string pos_name = _action_data.at( "position_name" ).asString();
                do {
                    ValueMap area = battle_layer->getMapData()->getAreaMapByName( pos_name );
                    if( area.empty() ) {
                        break;
                    }
                    const ValueMap& rect_data = area.at( "rect" ).asValueMap();
                    float x = rect_data.at( "x" ).asFloat();
                    float y = rect_data.at( "y" ).asFloat();
                    float width = rect_data.at( "width" ).asFloat();
                    float height = rect_data.at( "height" ).asFloat();
                    Point to_pos = Point( x + width / 2, y + height / 2 );
                    path->steps.push_back( to_pos );
                    auto itr = area.find( "prev_pos" );
                    if( itr != area.end() ) {
                        pos_name = itr->second.asString();
                    }
                    else {
                        break;
                    }
                }while( true );
                u->setTourPath( path );
                if( unit_state == UNIT_STATE_MOVE_TO ) {
                    u->setConcentrateOnWalk( true );
                }
            }
            else if( unit_state == UNIT_STATE_DIE ) {
                
            }
            else if( unit_state == UNIT_STATE_DISAPPEAR ) {
                u->changeUnitState( eUnitState::Disappear, true );
            }
            if( change_show_hp ) {
                if( show_hp ) {
                    u->showHP();
                }
                else {
                    u->hideHP();
                }
            }
            if( tag_name != "" ) {
                u->setUnitTags( tag_name );
            }
            if( unit_type != "" ) {
                u->setTargetCamp( UnitNode::getCampByString( unit_type ) );
            }
            if( item_name.size() > 0 ) {
                if( get_or_lose_item ) {
                    ValueMap item_data;
                    item_data["item_name"] = Value( item_name );
                    item_data["item_resource"] = Value( item_resource );
                    
                    Item* item = Item::create( item_data );
                    u->addItem( item );
                }
                else {
                    u->removeItem( item_name );
                }
            }
            if( !buff_data.empty() ) {
                Buff* buff = Buff::create( u, buff_data );
                u->addBuff( buff->getBuffId(), buff, true );
                buff->begin();
            }
        }
    }
}

//task change action
TaskChangeAction::TaskChangeAction() {
    
}

TaskChangeAction::~TaskChangeAction() {
    
}

TaskChangeAction* TaskChangeAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    TaskChangeAction* ret = new TaskChangeAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TaskChangeAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( TaskChangeAction::onActionTriggered, this );
    return true;
}

void TaskChangeAction::onActionTriggered( bool finish ) {
    EventAction::onActionTriggered( finish );
    
    const std::string& task_name = _action_data.at( "task_name" ).asString();
    const std::string& task_state = _action_data.at( "task_state" ).asString();
    float progress = 0;
    auto itr = _action_data.find( "progress" );
    if( itr != _action_data.end() ) {
        progress = itr->second.asFloat();
    }
    _map_logic->onTaskStateChanged( task_name, task_state, progress );
}

//game change action
GameChangeAction::GameChangeAction() {
}

GameChangeAction::~GameChangeAction() {
    
}

GameChangeAction* GameChangeAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    GameChangeAction* ret = new GameChangeAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GameChangeAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( GameChangeAction::onActionTriggered, this );
    return true;
}

void GameChangeAction::onActionTriggered( bool finish ) {
    const std::string& game_state = _action_data.at( "game_state" ).asString();
    _map_logic->getBattleLayer()->changeState( BattleLayer::getBattleStateFromString( game_state ) );
}

//event change action
EventChangeAction::EventChangeAction() {
    
}

EventChangeAction::~EventChangeAction() {
    
}

EventChangeAction* EventChangeAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    EventChangeAction* ret = new EventChangeAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EventChangeAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( EventChangeAction::onActionTriggered, this );
    return true;
}

void EventChangeAction::onActionTriggered( bool finish ) {
    EventAction::onActionTriggered( finish );
    
    const std::string& event_name = _action_data.at( "event_name" ).asString();
    int event_state = 0;
    std::string state_string = _action_data.at( "event_state" ).asString();
    if( state_string == EVENT_STATE_ENABLE ) {
        event_state = 0;
    }
    else if( state_string == EVENT_STATE_DISABLE ) {
        event_state = 1;
    }
    else if( state_string == EVENT_STATE_FINISH ) {
        event_state = 2;
    }
    _map_logic->setActionStateByName( event_name, event_state );
    _map_logic->setTriggerStateByName( event_name, event_state );
}

HintChangeAction::HintChangeAction() {
    
}

HintChangeAction::~HintChangeAction() {
    
}
    
HintChangeAction* HintChangeAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    HintChangeAction* ret = new HintChangeAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool HintChangeAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( HintChangeAction::onActionTriggered, this );
    return true;
}
    
void HintChangeAction::onActionTriggered( bool finish ) {
    EventAction::onActionTriggered( finish );
    
    const std::string& hint_name = _action_data.at( "hint_name" ).asString();
    bool visible = _action_data.at( "hint_visible" ).asBool();
    
    _map_logic->setHintVisibleByName( hint_name, visible );
}

//vision change action
VisionChangeAction::VisionChangeAction() {
    
}

VisionChangeAction::~VisionChangeAction() {
}

VisionChangeAction* VisionChangeAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    VisionChangeAction* ret = new VisionChangeAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool VisionChangeAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( VisionChangeAction::onActionTriggered, this );
    return true;
}

void VisionChangeAction::onActionTriggered( bool finish ) {
    std::string vision_state = _action_data.at( "vision_state" ).asString();
    bool enabled = true;
    if( vision_state == VISION_STATE_ENABLED ) {
        enabled = true;
    }
    else {
        enabled = false;
    }
    
    std::string source_type = _action_data.at( "source_type" ).asString();
    std::string source_value = _action_data.at( "source_value" ).asString();
    
    ValueMap update_data;
    update_data["trigger_type"] = Value( "vision_change" );
    update_data["source_type"] = Value( source_type );
    update_data["source_value"] = Value( source_value );
    update_data["vision_state"] = Value( vision_state );
    
    if( source_type == UNIT_SOURCE_NAME ) {
        const BlockMap& block_nodes = _map_logic->getBattleLayer()->getBlockNodes();
        for( auto itr = block_nodes.begin(); itr != block_nodes.end(); ++itr ) {
            BlockNode* block_node = itr->second;
            if( block_node->getBlockName() == source_value ) {
                itr->second->setEnabled( enabled );
                block_node->updateEnabled();
                _map_logic->onVisionChanged( update_data );
            }
        }
    }
}

//wave action
WaveAction::WaveAction() {
    
}

WaveAction::~WaveAction() {
    
}

WaveAction* WaveAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    WaveAction* ret = new WaveAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool WaveAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( WaveAction::onActionTriggered, this );
    return true;
}

void WaveAction::onActionTriggered( bool finish ) {
    
}

//conversation action
ConversationAction::ConversationAction() {
    
}

ConversationAction::~ConversationAction() {
    
}

ConversationAction* ConversationAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    ConversationAction* ret = new ConversationAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ConversationAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( ConversationAction::onActionTriggered, this );
    
    _current_speech_id = 0;
    _repeat_times = action_data.at( "repeat_times" ).asInt();
    _current_times = 0;
    _is_random_order = action_data.at( "is_random_order" ).asBool();
    
    return true;
}

void ConversationAction::onActionTriggered( bool finish ) {
    std::string source_type = _action_data.at( "source_type" ).asString();
    std::string source_value = _action_data.at( "source_value" ).asString();
    Vector<UnitNode*> candidates;
    if( source_type == UNIT_SOURCE_TYPE ) {
        candidates = _map_logic->getBattleLayer()->getAliveUnitsByCamp( UnitNode::getCampByString( source_value ) );
    }
    else if( source_type == UNIT_SOURCE_TAG ) {
        candidates = _map_logic->getBattleLayer()->getAliveUnitsByTag( source_value );
    }
    else if( source_type == UNIT_SOURCE_NAME ) {
        candidates = _map_logic->getBattleLayer()->getAliveUnitsByName( source_value );
    }
    if( candidates.size() == 0 ) {
        this->stop();
    }
    else {
        const ValueVector& speeches = _action_data.at( "speeches" ).asValueVector();
        int speech_count = (int)speeches.size();
        if( _is_random_order ) {
            _current_speech_id = Utils::randomNumber( speech_count ) - 1;
        }
        const ValueMap& speech_data = speeches.at( _current_speech_id ).asValueMap();
        float duration = speech_data.at( "duration" ).asFloat();
        std::string content = speech_data.at( "content" ).asString();
        //speech
        for( auto unit : candidates ) {
            unit->makeSpeech( content, duration );
        }
        
        _interval = speech_data.at( "interval" ).asFloat();
        
        if( _is_random_order ) {
            ++_current_times;
        }
        else {
            ++_current_speech_id;
            if( _current_speech_id >= speech_count ) {
                ++_current_times;
            }
        }
        if( _current_times >= _repeat_times ) {
            this->stop();
        }
    }
}

void ConversationAction::updateFrame( float delta ) {
    if( !_should_recycle && _is_running ) {
        _accumulator += delta;
        if( ( _current_round == 0 && _accumulator > _delay ) || ( _current_round > 0 && _accumulator > _interval ) ) {
            _accumulator = 0;
            if( _callback ) {
                _callback( false );
            }
            else {
                this->stop();
            }
        }
    }
}

StoryAction::StoryAction() {
    
}

StoryAction::~StoryAction() {
    
}

StoryAction* StoryAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    StoryAction* ret = new StoryAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool StoryAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( StoryAction::onActionTriggered, this );
    return true;
}

void StoryAction::onActionTriggered( bool finish ) {
    _map_logic->getBattleLayer()->startStory( _action_data );
}

CustomAction::CustomAction() {
    
}

CustomAction::~CustomAction() {
    
}

CustomAction* CustomAction::create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    CustomAction* ret = new CustomAction();
    if( ret && ret->init( action_data, map_logic, trigger ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CustomAction::init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger ) {
    if( !EventAction::init( action_data, map_logic, trigger ) ) {
        return false;
    }
    
    _callback = CC_CALLBACK_1( CustomAction::onActionTriggered, this );
    return true;
}

void CustomAction::onActionTriggered( bool finish ) {
    
}