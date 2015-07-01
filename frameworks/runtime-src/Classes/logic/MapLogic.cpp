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
#include "../util/CocosUtils.h"
#include "../constant/BoidsConstant.h"
#include "../manager/ResourceManager.h"

using namespace cocos2d;

MapLogic::MapLogic() {
    
}

MapLogic::~MapLogic() {
    
}

MapLogic* MapLogic::create( BattleLayer* battle_layer ) {
    MapLogic* ret = new MapLogic();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
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
    
    auto sitr = meta_json.find( "tasks" );
    if( sitr != meta_json.end() ) {
        int i = 0;
        const ValueVector& task_json = sitr->second.asValueVector();
        for( auto itr = task_json.begin(); itr != task_json.end(); ++itr ) {
            const ValueMap& t = itr->asValueMap();
            GameTask* gt = GameTask::create( t, this );
            _game_tasks.pushBack( gt );
            gt->setTaskId( i++ );
        }
    }
    
    sitr = meta_json.find( "events" );
    if( sitr != meta_json.end() ) {
        const ValueVector& events_json = sitr->second.asValueVector();
        for( auto itr = events_json.begin(); itr != events_json.end(); ++itr ) {
            EventTrigger* trigger = EventTrigger::create( this, itr->asValueMap() );
            _triggers.pushBack( trigger );
        }
    }
    
    const ValueMap& all_drop_config = ResourceManager::getInstance()->getDropConfig();
    sitr = ResourceManager::getInstance()->getDropConfig().find( _battle_layer->getLevelId() );
    if( sitr != all_drop_config.end() ) {
        const ValueMap& drop_config = sitr->second.asValueMap();
        int exp = drop_config.at( "exp" ).asInt();
        _obtained_items["exp"] = Value( exp );
        
        int gold = drop_config.at( "gold" ).asInt();
        _obtained_items["gold"] = Value( gold );
    }
    
    
    return true;
}

void MapLogic::updateFrame( float delta ) {
    this->updateEventActions( delta );
    this->updateEventTriggers( delta );
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
    
    bool is_leader = true;
    const ValueVector& player_units = PlayerInfo::getInstance()->getPlayerDeployedUnitsInfo();
    for( auto itr = player_units.begin(); itr != player_units.end(); ++itr ) {
        const ValueMap& vm = itr->asValueMap();
        UnitNode* unit = UnitNode::create( _battle_layer, vm );
        unit->setTargetCamp( eTargetCamp::Player );
        
        PlayerMoveBehavior* move_behavior = PlayerMoveBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_MOVE, move_behavior );
        IdleBehavior* idle_behavior = IdleBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_IDLE, idle_behavior );
        PlayerAttackBehavior* attack_behavior = PlayerAttackBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
        PlayerSkillBehavior* skill_behavior = PlayerSkillBehavior::create( unit );
        unit->addBehavior( BEHAVIOR_NAME_SKILL, skill_behavior );
        
        if( is_leader ) {
            is_leader = false;
            unit->setUnitTags( "player,leader" );
        }
        else {
            unit->setUnitTags( "player" );
        }
        
        cocos2d::Point pos = _battle_layer->getAvailablePosition( unit->getUnitData()->collide, player_start_area );
        _battle_layer->deployUnit( unit, pos, "player" );
    }
}

void MapLogic::onEventChanged( const std::string& event_name, const std::string& event_state ) {
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_EVENT_CHANGE );
            conditions["event_name"] = Value( event_name );
            conditions["event_state"] = Value( event_state );
            trigger->activateTriggerByConditions( conditions, nullptr );
        }
    }
}

void MapLogic::onMapInit() {
    this->deployPlayerUnits();
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_MAP_INIT );
            trigger->activateTriggerByConditions( conditions, nullptr );
        }
    }
}

void MapLogic::updateGameTime( float time ) {
    ValueMap update_data;
    update_data["game_time"] = Value( time );
    update_data["trigger_type"] = Value( EVENT_TRIGGER_TYPE_GAME_TIME );
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            trigger->activateTriggerByConditions( update_data, nullptr );
        }
    }
}

void MapLogic::onTaskStateChanged( const std::string& task_name, const std::string& task_state, float progress ) {
    for( auto task : _game_tasks ) {
        if( task->isActive() && task->getTaskName() == task_name ) {
            if( task_state == "task_progress" ) {
                task->addProgress( progress );
            }
            else {
                task->setTaskState( task_state );
            }
            if( task->isPrimary() && !task->isActive() ) {
                if( task->getTaskState() == GAME_TASK_STATE_FINISHED ) {
                    _battle_layer->changeState( eBattleState::BattleWin );
                }
                else if( task->getTaskState() == GAME_TASK_STATE_FAILED ) {
                    _battle_layer->changeState( eBattleState::BattleLose );
                }
            }
            break;
        }
    }
}

void MapLogic::onCustomTrigger( const std::string& trigger_name ) {
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            ValueMap conditions;
            conditions["trigger_type"] = Value( EVENT_TRIGGER_TYPE_CUSTOM );
            conditions["trigger_name"] = Value( trigger_name );
            trigger->activateTriggerByConditions( conditions, nullptr );
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
            trigger->activateTriggerByConditions( conditions, nullptr );
        }
    }
}

void MapLogic::onVisionChanged( const cocos2d::ValueMap& update_data ) {
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            trigger->activateTriggerByConditions( update_data, nullptr );
        }
    }
}

void MapLogic::addEventAction( EventAction* action, const std::string& key ) {
    _event_actions.insert( key, action );
}

void MapLogic::removeEventAction( const std::string& key ) {
    _event_actions.erase( key );
}

void MapLogic::setTriggerStateByName( const std::string& name, int state ) {
    for( auto trigger : _triggers ) {
        if( trigger->getEventData().at( "name" ).asString() == name ) {
            switch( state ) {
                case 0:
                    trigger->setEnabled( true );
                    break;
                case 1:
                    trigger->setEnabled( false );
                    break;
                case 2:
                    trigger->setShouldRecycle( true );
                    break;
                default:
                    break;
            }
        }
    }
}

void MapLogic::setActionStateByName( const std::string& name, int state ) {
    for( auto pair : _event_actions ) {
        EventAction* action = pair.second;
        if( action->getTriggerName() == name ) {
            switch( state ) {
                case 0:
                    action->resume();
                    break;
                case 1:
                    action->pause();
                    break;
                case 2:
                    action->stop();
                    break;
                default:
                    break;
            }
        }
    }
}

void MapLogic::setHintVisibleByName( const std::string& name, bool visible ) {
    _battle_layer->setHintVisibleByName( name, visible );
}

void MapLogic::onTargetNodeAppear( TargetNode* target_node ) {
    do {
        UnitNode* unit_node = dynamic_cast<UnitNode*>( target_node );
        if( unit_node ) {
            this->increaseUnitAppearCountByCamp( 1, (int)unit_node->getTargetCamp() );
            const cocos2d::ValueMap& tags = unit_node->getUnitTags();
            for( auto itr = tags.begin(); itr != tags.end(); ++itr ) {
                this->increaseUnitAppearCountByTag( 1, itr->second.asString() );
            }
            this->increaseUnitAppearCountByName( 1, unit_node->getUnitData()->name );
            
            for( auto trigger : _triggers ) {
                if( trigger->isEnabled() ) {
                    ValueMap area = _battle_layer->getMapData()->getAreaMapByPosition( unit_node->getPosition() );
                    if( !area.empty() ) {
                        trigger->activateTriggerByUnit( unit_node, UNIT_STATE_APPEAR, area );
                    }
                }
            }

            break;
        }
    }while( false );
}

void MapLogic::onTargetNodeDead( class TargetNode* target_node ) {
    do {
        UnitNode* unit_node = dynamic_cast<UnitNode*>( target_node );
        if( unit_node ) {
            //drop items
            if( unit_node->getTargetCamp() == eTargetCamp::Enemy ) {
                this->dropItem( unit_node );
            }
            
            //update trigger
            for( auto trigger : _triggers ) {
                if( trigger->isEnabled() ) {
                    trigger->activateTriggerByUnit( unit_node, UNIT_STATE_DEAD, ValueMap() );
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
            this->increaseUnitDisappearCountByCamp( 1, (int)unit_node->getTargetCamp() );
            const cocos2d::ValueMap& tags = unit_node->getUnitTags();
            for( auto itr = tags.begin(); itr != tags.end(); ++itr ) {
                this->increaseUnitDisappearCountByTag( 1, itr->second.asString() );
            }
            this->increaseUnitDisappearCountByName( 1, unit_node->getUnitData()->name );
            
            for( auto trigger : _triggers ) {
                if( trigger->isEnabled() ) {
                    ValueMap area = _battle_layer->getMapData()->getAreaMapByPosition( unit_node->getPosition() );
                    if( !area.empty() ) {
                        trigger->activateTriggerByUnit( unit_node, UNIT_STATE_DISAPPEAR, area );
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
                trigger->activateTriggerByUnit( unit_node, UNIT_STATE_MOVE_TO, area );
            }
        }
    }
}

void MapLogic::onStoryChange( const std::string& story_name, const std::string& story_state ) {
    ValueMap update_data;
    update_data["trigger_type"] = Value( EVENT_TRIGGER_TYPE_STORY_CHANGE );
    update_data["story_name"] = Value( story_name );
    update_data["story_state"] = Value( story_state );
    
    for( auto trigger : _triggers ) {
        if( trigger->isEnabled() ) {
            trigger->activateTriggerByConditions( update_data, nullptr );
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

void MapLogic::updateEventTriggers( float delta ) {
    auto itr = _triggers.begin();
    while( itr != _triggers.end() ) {
        if( (*itr)->shouldRecycle() ) {
            itr = _triggers.erase( itr );
        }
        else {
            (*itr)->updateFrame( delta );
            ++itr;
        }
    }
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

void MapLogic::obtainItem( const std::string& item_id, int count ) {
    auto itr = _obtained_items.find( item_id );
    if( itr != _obtained_items.end() ) {
        int old_count = itr->second.asInt();
        _obtained_items[item_id] = Value( count + old_count );
    }
    else {
        _obtained_items.insert( std::make_pair( item_id, Value( count ) ) );
    }
}

void MapLogic::dropItem( UnitNode* unit ) {
    const ValueMap& all_drop_config = ResourceManager::getInstance()->getDropConfig();
    auto itr = all_drop_config.find( _battle_layer->getLevelId() );
    if( itr != all_drop_config.end() ) {
        const ValueMap& drop_config = itr->second.asValueMap();
        Point pos = unit->getHitPos();
        
        float rand;
        float rate;
        int count;
        if( unit->isBoss() ) {
            rate = drop_config.at( "rate_boss" ).asFloat();
            count = drop_config.at( "drop_gold" ).asInt() * 2;
        }
        else {
            rate = drop_config.at( "rate_normal" ).asFloat();
            count = drop_config.at( "drop_gold" ).asInt();
        }
        
        rand = Utils::randomFloat();
        if( rand < rate ) {
            count = Utils::randomNumber( count );
            ValueMap item_data;
            item_data["item_id"] = Value( "gold" );
            item_data["count"] = Value( count );
            DropItem* item = DropItem::create( item_data );
            _battle_layer->dropItem( item, pos, eBattleSubLayer::ObjectLayer );
            
            Point drop_pos = Utils::randomPositionInRange( unit->getPosition(), 50.0f );
            Rect region = Rect( drop_pos.x - 25.0f, drop_pos.y - 25.0f, 50.0f, 50.0 );
            Point desired_pos = _battle_layer->getAvailablePosition( 50.0f, region );
            if( desired_pos.equals( Point::ZERO ) ) {
                desired_pos = unit->getPosition();
            }
            item->dropTo( desired_pos );
        }
        
        const ValueVector& items = drop_config.at( "items" ).asValueVector();
        for( auto v : items ) {
            const ValueMap& config = v.asValueMap();
            if( unit->isBoss() ) {
                rate = config.at( "rate_boss" ).asFloat();
            }
            else {
                rate = config.at( "rate_normal" ).asFloat();
            }
            
            rand = Utils::randomFloat();
            if( rand < rate ) {
                ValueMap item_data;
                item_data["item_id"] = config.at( "item_id" );
                item_data["count"] = Value( 1 );
                DropItem* item = DropItem::create( item_data );
                _battle_layer->dropItem( item, pos, eBattleSubLayer::ObjectLayer );
                
                Point drop_pos = Utils::randomPositionInRange( pos, 50.0f );
                Rect region = Rect( drop_pos.x - 25.0f, drop_pos.y - 25.0f, 50.0f, 50.0 );
                Point desired_pos = _battle_layer->getAvailablePosition( 50.0f, region );
                if( desired_pos.equals( Point::ZERO ) ) {
                    desired_pos = unit->getPosition();
                }

                item->dropTo( desired_pos );
            }
        }
    }
}