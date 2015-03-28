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
        if( trigger->isEnabled() ) {
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
        if( trigger->isEnabled() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_MAP_INIT );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::onTaskStateChanged( const std::string& task_name, const std::string& task_state ) {
    
}

void MapLogic::onGameStateChanged( const std::string& game_state ) {
    _battle_layer->changeState( BattleLayer::getBattleStateFromString( game_state ) );
}

void MapLogic::onVisionChanged( const cocos2d::ValueMap& action_data ) {
    
}

void MapLogic::onCustomTrigger( const std::string& trigger_name ) {
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_CUSTOM );
            conditions["trigger_name"] = Value( trigger_name );
            trigger->activateTriggerByConditions( conditions, this, nullptr );
        }
    }
}

void MapLogic::onConversationStateChanged( const std::string& trigger_name, const std::string& trigger_state ) {
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
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
        if( trigger->getEventData().at( "name" ).asString() == name ) {
            trigger->setEnabled( b );
        }
    }
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
            
            for( auto trigger : _triggers ) {
                if( trigger->isEnabled() ) {
                    ValueMap area = _battle_layer->getMapData()->getAreaMapByPosition( unit_node->getPosition() );
                    if( !area.empty() ) {
                        trigger->activateTriggerByUnit( this, unit_node, UNIT_STATE_APPEAR, area );
                    }
                }
            }

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
            
            for( auto trigger : _triggers ) {
                if( trigger->isEnabled() ) {
                    ValueMap area = _battle_layer->getMapData()->getAreaMapByPosition( unit_node->getPosition() );
                    if( !area.empty() ) {
                        trigger->activateTriggerByUnit( this, unit_node, UNIT_STATE_MOVE_TO, area );
                    }
                }
            }
            break;
        }
    }while( false );
}

void MapLogic::onUnitMoved( class UnitNode* unit_node ) {
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            ValueMap area = _battle_layer->getMapData()->getAreaMapByPosition( unit_node->getPosition() );
            if( !area.empty() ) {
                trigger->activateTriggerByUnit( this, unit_node, UNIT_STATE_MOVE_TO, area );
            }
        }
    }
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