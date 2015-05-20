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
#include "../manager/ResourceManager.h"

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

const cocos2d::ValueMap& PlayerInfo::getAllUnitsInfo() {
    return _player_info.at( "units" ).asValueMap();
}

const cocos2d::ValueMap& PlayerInfo::getPlayerDeployedUnitsSlotIds() {
    return _player_info.at( "deploy_units" ).asValueMap();
}

cocos2d::ValueVector PlayerInfo::getPlayerDeployedUnitsInfo() {
    ValueVector ret;
    const ValueMap& player_units = _player_info.at( "units" ).asValueMap();
    const ValueMap& player_unit_ids = this->getPlayerDeployedUnitsSlotIds();
    
    std::vector<std::string> keys;
    for( auto pair : player_unit_ids ) {
        keys.push_back( pair.first );
    }
    std::sort( keys.begin(), keys.end(), [&]( const std::string& a, const std::string& b )->bool{ return Utils::toInt( a ) < Utils::toInt( b ); } );
    
    for( auto key : keys ) {
        std::string slot_id = player_unit_ids.at( key ).asString();
        if( slot_id != "l" & slot_id != "0" ) {
            auto sitr = player_units.find( slot_id );
            if( sitr != player_units.end() ) {
                ret.push_back( sitr->second );
            }
        }
    }
    return ret;
}

cocos2d::ValueVector PlayerInfo::getPlayerDeployedUnitNames() {
    ValueVector ret;
    const ValueMap& player_units = _player_info.at( "units" ).asValueMap();
    const ValueMap& player_unit_ids = this->getPlayerDeployedUnitsSlotIds();
    
    std::vector<std::string> keys;
    for( auto pair : player_unit_ids ) {
        keys.push_back( pair.first );
    }
    std::sort( keys.begin(), keys.end(), [&]( const std::string& a, const std::string& b )->bool{ return Utils::toInt( a ) < Utils::toInt( b ); } );
    
    for( auto key : keys ) {
        std::string slot_id = player_unit_ids.at( key ).asString();
        if( slot_id != "l" && slot_id != "0" ) {
            auto sitr = player_units.find( slot_id );
            if( sitr != player_units.end() ) {
                ret.push_back( sitr->second.asValueMap().at( "name" ) );
            }
        }
    }
    return ret;
}

void PlayerInfo::setDeployUnit( const std::string& slot_id, const std::string& hero_id ) {
    ValueMap& player_unit_ids = _player_info.at( "deploy_units" ).asValueMap();
    auto itr = player_unit_ids.find( slot_id );
    if( itr != player_unit_ids.end() ) {
        itr->second = Value( hero_id );
    }
}

cocos2d::ValueMap PlayerInfo::upgradeHero( const std::string& hero_id, int level ) {
    ValueMap& all_units = _player_info.at( "units" ).asValueMap();
    auto itr = all_units.find( hero_id );
    if( itr != all_units.end() ) {
        ValueMap& unit_data = all_units.at( hero_id ).asValueMap();
        int old_level = unit_data["level"].asInt();
        unit_data["level"] = Value( old_level + level );
        return unit_data;
    }
    return ValueMap();
}

void PlayerInfo::gainGold( int gain ) {
    int gold = this->getGold();
    _player_info["gold"] = Value( gold + gain );
}

int PlayerInfo::getGold() {
    return _player_info.at( "gold" ).asInt();
}

void PlayerInfo::gainDiamond( int gain ) {
    int diamond = this->getDiamond();
    _player_info["diamond"] = Value( diamond + gain );
}

int PlayerInfo::getDiamond() {
    return _player_info.at( "diamond" ).asInt();
}

void PlayerInfo::gainTeamExp( int exp ) {
    int team_exp = _player_info.at( "team_exp" ).asInt();
    team_exp += exp;
    _player_info["team_exp"] = Value( team_exp );
    int team_level = this->getTeamLevel();
    int next_team_level = team_level + 1;
    const ValueVector& team_level_exp_conf = ResourceManager::getInstance()->getTeamLevelExpConfig();
    if( team_level_exp_conf.size() > next_team_level ) {
        int next_team_level_exp = team_level_exp_conf[next_team_level-1].asInt();
        if( team_exp >= next_team_level_exp ) {
            this->setTeamLevel( next_team_level );
        }
    }
}

void PlayerInfo::setTeamLevel( int level ) {
    _player_info["team_level"] = Value( level );
}

int PlayerInfo::getTeamLevel() {
    return _player_info.at( "team_level" ).asInt();
}

void PlayerInfo::gainTeamSkillLevel( int lvl_up, const std::string& skill_name ) {
    ValueMap& skill = this->getTeamSkill( skill_name );
    int sk_lvl = skill["level"].asInt();
    skill["level"] = Value( sk_lvl + lvl_up );
}

ValueMap& PlayerInfo::getTeamSkill( const std::string& skill_name ) {
    return _player_info.at( "team_skills" ).asValueMap().at( skill_name ).asValueMap();
}

int PlayerInfo::getLevelUpCost( const std::string& slot ) {
    //todo
    return INT_MAX;
}

int PlayerInfo::unitLevelUpByOne( const std::string& slot ) {
    int team_level = _player_info.at( "team_level" ).asInt();
    int gold = _player_info.at( "gold" ).asInt();
    ValueMap& player_units = _player_info.at( "units" ).asValueMap();
    auto itr = player_units.find( slot );
    if( itr != player_units.end() ) {
        ValueMap& unit_data = itr->second.asValueMap();
        int unit_level = unit_data.at( "level" ).asInt();
        if( unit_level >= team_level ) {
            return LEVEL_UP_ERROR_REACH_LEVEL_LIMIT;
        }
        const ValueVector& levelup_cost = ResourceManager::getInstance()->getUnitLevelupCostConfig().at( unit_data.at( "name" ).asString() ).asValueVector();
        if( levelup_cost.size()  <= unit_level ) {
            return LEVEL_UP_ERROR_REACH_LEVEL_LIMIT;
        }
        int cost = levelup_cost.at( unit_level ).asInt();
        if( gold < cost ) {
            return LEVEL_UP_ERROR_NOT_ENOUGH_GOLD;
        }
        gold -= cost;
        ++unit_level;
        unit_data["level"] = Value( unit_level );
        _player_info["gold"] = Value( "gold" );
        return LEVEL_UP_SUCCESS;
    }
    return LEVEL_UP_ERROR_NOT_FOUND;
}