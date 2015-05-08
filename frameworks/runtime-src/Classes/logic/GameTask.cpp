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

GameTask::GameTask() :
_progress( 0 )
{
    
}

GameTask::~GameTask() {
    
}

GameTask* GameTask::create( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    GameTask* ret = new GameTask();
    if( ret && ret->init( data, map_logic ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GameTask::init( const cocos2d::ValueMap& data, MapLogic* map_logic ) {
    _map_logic = map_logic;
    _state = GAME_TASK_STATE_STARTED;
    _is_active = true;
    
    _task_name = data.at( "name" ).asString();
    _task_title = data.at( "title" ).asString();
    _task_desc = data.at( "desc" ).asString();
    _is_primary = ( data.at( "type" ).asString() == "primary_win" );
    _progress = 0;
    
    return true;
}

void GameTask::setTaskState( const std::string& new_state ) {
    _state = new_state;
    if( _state == GAME_TASK_STATE_STARTED ) {
        this->setActive( true );
    }
    else {
        this->setActive( false );
    }
}

void GameTask::addProgress( float p ) {
    _progress += p;
    if( _progress >= 100.0f ) {
        _progress = 100.0f;
        this->setTaskState( GAME_TASK_STATE_FINISHED );
    }
}