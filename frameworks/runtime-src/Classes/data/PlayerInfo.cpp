//
//  PlayerInfo.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "PlayerInfo.h"
#include "external/json/Document.h"
#include "../util/CocosUtils.h"

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
    rapidjson::Document player_info_json;
    player_info_json.Parse<0>( data_string.c_str() );
    _player_info = CocosUtils::jsonObjectToValueMap( player_info_json );
}

const cocos2d::ValueVector& PlayerInfo::getPlayerUnitsInfo() {
    return _player_info.at( "units" ).asValueVector();
}

cocos2d::ValueVector PlayerInfo::getPlayerUnitNames() {
    ValueVector ret;
    const ValueVector& units = _player_info.at( "units" ).asValueVector();
    for( auto itr = units.begin(); itr != units.end(); ++itr ) {
        ret.push_back( Value( itr->asValueMap().at( "name" ).asString() ) );
    }
    return ret;
}