//
//  BETypes.cpp
//  Boids
//
//  Created by Xin Xu on 11/27/14.
//
//

#include "BETypes.h"

EditorTypeManager *EditorTypeManager::_instance = nullptr;

EditorTypeManager *EditorTypeManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new EditorTypeManager();
    }
    return _instance;
}

EditorTypeManager::EditorTypeManager() {
    _eventTriggerState = EditorTypePtr(new EditorType());
    _eventActionState = EditorTypePtr(new EditorType());
    _gameState = EditorTypePtr(new EditorType());
    _taskState = EditorTypePtr(new EditorType());
    _unitActionState = EditorTypePtr(new EditorType());
    _unitTriggerState = EditorTypePtr(new EditorType());
    _unitType = EditorTypePtr(new EditorType());
    _unitTriggerSourceType = EditorTypePtr(new EditorType());
    _unitActionSourceType = EditorTypePtr(new EditorType());
    _triggerType = EditorTypePtr(new EditorType());
    _actionType = EditorTypePtr(new EditorType());
    _visionObjectStateType = EditorTypePtr(new EditorType());
    _visionObjectSourceType = EditorTypePtr(new EditorType());
    _gameConditionType = EditorTypePtr(new EditorType());
    _gameConditionSourceType = EditorTypePtr(new EditorType());
    _conversationState = EditorTypePtr(new EditorType());
    _unitPopupType = EditorTypePtr(new EditorType());
    _triggerRelationType = EditorTypePtr(new EditorType());
    _eventTriggerState->_typeVector = { "event_trigger", "event_finish" };
    _eventActionState->_typeVector = { "event_disable", "event_enable" };
    _gameState->_typeVector = { "game_start", "game_win", "game_lose" };
    _taskState->_typeVector = { "task_start", "task_finish", "task_failed" };
    _unitActionState->_typeVector = { "unit_appear", "unit_hold_appear", "unit_move", "unit_patrol", "unit_disappear", "unit_idle", "unit_die" };
    _unitTriggerState->_typeVector = { "unit_appear", "unit_move", "unit_disappear" };
    _unitType->_typeVector = { "player", "neutral", "ally", "enemy", "item", "wild" };
    _unitTriggerSourceType->_typeVector = { "type_source", "tag_source", "name_source" };
    _unitActionSourceType->_typeVector = { "trigger_source", "type_source", "tag_source", "name_source" };
    _triggerType->_typeVector = { "map_init", "unit_change", "unit_stay", "event_change", "game_change", "vision_change", "conversation_change", "custom" };
    _actionType->_typeVector = { "unit_change", "task_change", "game_change", "event_change", "vision_change", "custom", "wave_action", "conversation_action" };
    _visionObjectStateType->_typeVector = { "unnormal", "normal", "disable" };
    _visionObjectSourceType->_typeVector = { "name_source", "tag_source" };
    _gameConditionType->_typeVector = { "player_nodie", "unit_die", "unit_alive", "clear_enemy", "arrive_area", "collect_item", "time_limit" };
    _gameConditionSourceType->_typeVector = { "primary_win", "additional_win", "lose" };
    _conversationState->_typeVector = { "conversation_start", "conversation_end" };
    _unitPopupType->_typeVector = { "normal", "help", "yellow_surprise", "red_surprise" };
    _triggerRelationType->_typeVector = { "then", "and", "or" };
}