//
//  PlayerInfo.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "PlayerInfo.h"
#include "cocos2d.h"

using namespace cocos2d;

PlayerInfo* PlayerInfo::_instance = nullptr;

PlayerInfo::PlayerInfo() {
    
}

PlayerInfo::~PlayerInfo() {
    
}

PlayerInfo* PlayerInfo::getInstance() {
    if( _instance == nullptr ) {
        _instance = new PlayerInfo();
    }
    return _instance;
}

void PlayerInfo::loadPlayerInfo() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "player_info.json" );
    _player_info.Parse<0>( data_string.c_str() );
}

const rapidjson::Value& PlayerInfo::getPlayerUnitsInfo() {
    return _player_info["units"];
}