//
//  GameTask.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/23/15.
//
//

#include "GameTask.h"
#include "../constant/BoidsConstant.h"
#include "MapLogic.h"

using namespace cocos2d;

GameTask::GameTask() {
    
}

GameTask::~GameTask() {
    
}

GameTask* GameTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    GameTask* ret = nullptr;
    std::string type = data.at( "task_type" ).asString();
    if( type == GAME_TASK_TYPE_ELIMINATE_UNIT ) {
        ret = EliminateUnitTask::create( data, map_logic );
    }
    else if( type == GAME_TASK_TYPE_LIMITED_TIME ) {
        ret = LimitedTimeTask::create( data, map_logic );
    }
    else if( type == GAME_TASK_TYPE_SURVIVE ) {
        ret = SurviveTask::create( data, map_logic );
    }
    else if( type == GAME_TASK_TYPE_ESCAPE ) {
        ret = EscapeTask::create( data, map_logic );
    }
    else if( type == GAME_TASK_TYPE_COLLECT_ITEM ) {
        ret = CollectItemTask::create( data, map_logic );
    }
    return ret;
}

bool GameTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    _state = GAME_TASK_STATE_UNSTARTED;
    _is_active = false;
    return true;
}

void GameTask::updateFrame( float delta ) {
    if( _is_active ) {
        
    }
}

void GameTask::setTaskState( const std::string& new_state ) {
    _state = new_state;
    if( _state == GAME_TASK_STATE_STARTED ) {
        _is_active = true;
    }
    else {
        _is_active = false;
    }
}

LimitedTimeTask::LimitedTimeTask() {
    
}

LimitedTimeTask::~LimitedTimeTask() {
    
}

LimitedTimeTask* LimitedTimeTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    LimitedTimeTask* ret = new LimitedTimeTask();
    if( ret && ret->init( data, map_logic ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool LimitedTimeTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    return true;
}

SurviveTask::SurviveTask() {
    
}

SurviveTask::~SurviveTask() {
    
}

SurviveTask* SurviveTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    SurviveTask* ret = new SurviveTask();
    if( ret && ret->init( data, map_logic ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SurviveTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    return true;
}

EliminateUnitTask::EliminateUnitTask() {
    
}

EliminateUnitTask::~EliminateUnitTask() {
    
}

EliminateUnitTask* EliminateUnitTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    EliminateUnitTask* ret = new EliminateUnitTask();
    if( ret && ret->init( data, map_logic ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EliminateUnitTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    return true;
}

CollectItemTask::CollectItemTask() {
    
}

CollectItemTask::~CollectItemTask() {
    
}

CollectItemTask* CollectItemTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    CollectItemTask* ret = new CollectItemTask();
    if( ret && ret->init( data, map_logic ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CollectItemTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    return true;
}

EscapeTask::EscapeTask() {
    
}

EscapeTask::~EscapeTask() {
    
}

EscapeTask* EscapeTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    EscapeTask* ret = new EscapeTask();
    if( ret && ret->init( data, map_logic ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EscapeTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    return true;
}
