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

#define PLAYER_INFO_DATA_FILE "player_info"

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
    FileUtils* file_util = FileUtils::getInstance();
    std::string plist_file = FileUtils::getInstance()->getWritablePath() + PLAYER_INFO_DATA_FILE + ".plist";
    if( !file_util->isFileExist( plist_file ) ) {
        std::string data_string = FileUtils::getInstance()->getStringFromFile( "player_info.json" );
        rapidjson::Document player_info_json;
        player_info_json.Parse<0>( data_string.c_str() );
        _player_info = CocosUtils::jsonObjectToValueMap( player_info_json );
        file_util->writeToFile( _player_info, plist_file );
    }
    else {
        _player_info = file_util->getValueMapFromFile( plist_file );
    }
}

void PlayerInfo::recordPlayerInfo() {
    FileUtils* file_util = FileUtils::getInstance();
    std::string plist_file = FileUtils::getInstance()->getWritablePath() + PLAYER_INFO_DATA_FILE + ".plist";
    file_util->writeToFile( _player_info, plist_file );
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
        player_unit_ids.at( slot_id ) = Value( hero_id );
    }
    this->recordPlayerInfo();
}

void PlayerInfo::setDeployUnits( const cocos2d::ValueMap& data ) {
    ValueMap& player_unit_ids = _player_info.at( "deploy_units" ).asValueMap();
    for( auto pair : data ) {
        auto itr = player_unit_ids.find( pair.first );
        if( itr != player_unit_ids.end() ) {
            player_unit_ids.at( pair.first ) = pair.second;
        }
    }
    this->recordPlayerInfo();
}

cocos2d::ValueMap PlayerInfo::updateUnitEquip( const std::string& hero_id, const std::string& type, const std::string& obj_id ) {
    ValueMap& all_heros = _player_info.at( "units" ).asValueMap();
    auto itr = all_heros.find( hero_id );
    if( itr != all_heros.end() ) {
        ValueMap& hero_data = all_heros.at( hero_id ).asValueMap();
        ValueMap& equip_data = hero_data.at( "equips" ).asValueMap();
        itr = equip_data.find( type );
        if( itr != equip_data.end() ) {
            equip_data.at( type ) = Value( obj_id );
            this->recordPlayerInfo();
            return hero_data;
        }
    }
    return ValueMap();
}

void PlayerInfo::updateEquip( const std::string& obj_id, bool in_use ) {
    ValueMap& equip_repo = _player_info.at( "equips" ).asValueMap();
    auto itr = equip_repo.find( obj_id );
    if( itr != equip_repo.end() ) {
        equip_repo.at( obj_id ).asValueMap().at( "in_use" ) = Value( in_use );
        this->recordPlayerInfo();
    }
}

void PlayerInfo::addEquip( const std::string& obj_id, const std::string& equip_id ) {
    ValueMap& equip_repo = _player_info.at( "equips" ).asValueMap();
    ValueMap equip_data;
    equip_data["obj_id"] = Value( obj_id );
    equip_data["in_use"] = Value( false );
    equip_data["id"] = Value( equip_id );
    equip_repo.insert( std::make_pair( obj_id, Value( equip_data ) ) );
    this->recordPlayerInfo();
}

void PlayerInfo::removeEquip( const std::string& obj_id ) {
    ValueMap& equip_repo = _player_info.at( "equips" ).asValueMap();
    equip_repo.erase( obj_id );
    this->recordPlayerInfo();
}

cocos2d::ValueVector PlayerInfo::getEquipsByRange( int type, int from, int size, int order ) {
    ValueVector ret;
    const ValueMap& all_equips = _player_info.at( "equips" ).asValueMap();
    ValueVector all_data;
    for( auto pair : all_equips ) {
        const ValueMap& equip_data = pair.second.asValueMap();
        int equip_id = equip_data.at( "id" ).asInt();
        bool in_use = equip_data.at( "in_use" ).asBool();
        int equip_type = equip_id / 1e7;
        if( !in_use && type == equip_type ) {
            all_data.push_back( Value( equip_data ) );
        }
    }
    
    if( order == 1 ) {
        //ascending order
        std::sort( all_data.begin(), all_data.end(), [&]( const Value& v1, const Value& v2 )->bool {
            int equip_id_1 = v1.asValueMap().at( "id" ).asInt();
            int equip_id_2 = v2.asValueMap().at( "id" ).asInt();
            return equip_id_1 < equip_id_2;
        } );
    }
    else if( order == 2 ) {
        //descending order
        std::sort( all_data.begin(), all_data.end(), [&]( const Value& v1, const Value& v2 )->bool {
            int equip_id_1 = v1.asValueMap().at( "id" ).asInt();
            int equip_id_2 = v2.asValueMap().at( "id" ).asInt();
            return equip_id_1 > equip_id_2;
        } );
    }
    
    int cur_id = from;
    int total_count = all_data.size();
    if( from < total_count ) {
        while( cur_id < total_count && cur_id <= from + size ) {
            ret.push_back( all_data.at( cur_id ) );
            ++cur_id;
        }
    }
    
    return ret;
}

cocos2d::ValueMap PlayerInfo::upgradeHero( const std::string& hero_id, int level ) {
    ValueMap& all_units = _player_info.at( "units" ).asValueMap();
    auto itr = all_units.find( hero_id );
    if( itr != all_units.end() ) {
        ValueMap& unit_data = all_units.at( hero_id ).asValueMap();
        int old_level = unit_data["level"].asInt();
        unit_data["level"] = Value( old_level + level );
        this->recordPlayerInfo();
        return unit_data;
    }
    return ValueMap();
}

cocos2d::ValueMap PlayerInfo::upgradeSkill( const std::string& hero_id, const std::string& skill_name, int level ) {
    ValueMap& all_units = _player_info.at( "units" ).asValueMap();
    auto itr = all_units.find( hero_id );
    if( itr != all_units.end() ) {
        ValueMap& unit_data = all_units.at( hero_id ).asValueMap();
        ValueVector& skill_data_vector = unit_data.at( "skills" ).asValueVector();
        for( int i = 0; i < skill_data_vector.size(); i++ ) {
            ValueMap& skill_data = skill_data_vector.at( i ).asValueMap();
            if( skill_data.at( "name" ).asString() == skill_name ) {
                int old_level = skill_data.at( "level" ).asInt();
                skill_data["level"] = Value( old_level + level );
                this->recordPlayerInfo();
                return unit_data;
            }
        }
    }
    return ValueMap();
}

const cocos2d::ValueMap& PlayerInfo::getAllEquipsInfo() {
    return _player_info["equips"].asValueMap();
}

void PlayerInfo::gainGold( int gain ) {
    int gold = this->getGold();
    _player_info["gold"] = Value( gold + gain );
    this->recordPlayerInfo();
}

int PlayerInfo::getGold() {
    return _player_info.at( "gold" ).asInt();
}

void PlayerInfo::gainDiamond( int gain ) {
    int diamond = this->getDiamond();
    _player_info["diamond"] = Value( diamond + gain );
    this->recordPlayerInfo();
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
    const ValueMap& team_level_exp_conf = ResourceManager::getInstance()->getTeamLevelExpConfig();
    if( team_level_exp_conf.size() > next_team_level ) {
        
    }
}

void PlayerInfo::setTeamLevel( int level ) {
    _player_info["team_level"] = Value( level );
}

int PlayerInfo::getTeamLevel() {
    return _player_info.at( "team_level" ).asInt();
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

int PlayerInfo::getTotalTalentPoints() {
    return _player_info.at( "team_talent" ).asValueMap().at( "total_points" ).asInt();
}

void PlayerInfo::setTotalTalentPoints( int points ) {
    _player_info.at( "team_talent" ).asValueMap().at( "total_points" ) = Value( points );
    this->recordPlayerInfo();
}

ValueVector PlayerInfo::getTeamTalent( const std::string& type ) {
    return _player_info.at( "team_talent" ).asValueMap().at( type ).asValueVector();
}

void PlayerInfo::setTeamTalent( const std::string& type, const std::string& slot ) {
    ValueVector& talent = _player_info.at( "team_talent" ).asValueMap().at( type ).asValueVector();
    bool exist = false;
    for( auto v : talent ) {
        if( v.asString() == slot ) {
            exist = true;
            break;
        }
    }
    if( !exist ) {
        talent.push_back( Value( slot ) );
        this->recordPlayerInfo();
    }
}

void PlayerInfo::setTeamTalent( const std::string& type, const cocos2d::ValueVector& talent_vector ) {
    ValueVector& talent = _player_info.at( "team_talent" ).asValueMap().at( type ).asValueVector();
    talent.clear();
    for( auto v : talent_vector ) {
        talent.push_back( v );
    }
    this->recordPlayerInfo();
}

void PlayerInfo::resetTeamTalent( const std::string& type ) {
    ValueVector& talent = _player_info.at( "team_talent" ).asValueMap().at( type ).asValueVector();
    talent.clear();
    this->recordPlayerInfo();
}