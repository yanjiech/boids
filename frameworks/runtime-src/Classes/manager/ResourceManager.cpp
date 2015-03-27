//
//  ResourceManager.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "ResourceManager.h"
#include "../constant/BoidsConstant.h"
#include "../Utils.h"
#include "../ArmatureManager.h"
#include "cocostudio/CocoStudio.h"
#include "../data/PlayerInfo.h"
#include "external/json/document.h"
#include "../util/CocosUtils.h"

using namespace cocos2d;

ResourceManager* ResourceManager::_instance = nullptr;

ResourceManager::ResourceManager() {
    
}

ResourceManager::~ResourceManager() {
    
}

ResourceManager* ResourceManager::getInstance() {
    if( _instance == nullptr ) {
        _instance = new ResourceManager();
    }
    return _instance;
}

void ResourceManager::destroy() {
    if( _instance ) {
        delete _instance;
        _instance = nullptr;
    }
}

void ResourceManager::loadMap( MapData* map_data ) {
    map_data->loadImagesToCache();
    const ValueMap& meta_json = map_data->getMetaJson();
    
    const ValueVector& unit_names = meta_json.at( "units" ).asValueVector();
    this->loadUnitArmatures( unit_names );
    
    const ValueVector& player_unit_names = PlayerInfo::getInstance()->getPlayerDeployedUnitNames();
    this->loadUnitArmatures( player_unit_names );
}

void ResourceManager::purgeMap( MapData* map_data ) {
    map_data->removeImagesFromCache();
    const ValueMap& meta_json = map_data->getMetaJson();
    
    const ValueVector& unit_names = meta_json.at( "units" ).asValueVector();
    this->purgeUnitArmatures( unit_names );
    
    const ValueVector& player_unit_names = PlayerInfo::getInstance()->getPlayerDeployedUnitNames();
    this->purgeUnitArmatures( player_unit_names );
}

void ResourceManager::loadAllData() {
    this->loadDefaultData();
    this->loadUnitData();
    this->loadUnitLevelupCostData();
    this->loadBulletData();
    this->loadCenterData();
    this->loadBuildingData();
    this->loadBattleUIData();
    this->loadLevelData();
    this->loadSkillData();
    this->loadTeamLevelExpData();
}

void ResourceManager::loadDefaultData() {
    SpriteFrameCache* frame_cache = SpriteFrameCache::getInstance();
    frame_cache->addSpriteFramesWithFile( "maps/map_images/building_1.plist", "maps/map_images/building_1.png" );
    frame_cache->addSpriteFramesWithFile( "maps/map_images/building_2.plist", "maps/map_images/building_2.png" );
    frame_cache->addSpriteFramesWithFile( "maps/map_images/building_3.plist", "maps/map_images/building_3.png" );
    frame_cache->addSpriteFramesWithFile( "maps/map_images/building_4.plist", "maps/map_images/building_4.png" );
    frame_cache->addSpriteFramesWithFile( "maps/map_images/decoration_1.plist", "maps/map_images/decoration_1.png" );
    frame_cache->addSpriteFramesWithFile( "maps/map_images/obstacle_1.plist", "maps/map_images/obstacle_1.png" );
    frame_cache->addSpriteFramesWithFile( "maps/map_images/terrain_1.plist", "maps/map_images/terrain_1.png" );
    frame_cache->addSpriteFramesWithFile( "ui/map_common.plist", "ui/map_common.png" );
    frame_cache->addSpriteFramesWithFile( "ui/hero_avatars.plist", "ui/hero_avatars.png" );
    frame_cache->addSpriteFramesWithFile( "effects/bullets/bullets.plist", "effects/bullets/bullets.png" );
    frame_cache->addSpriteFramesWithFile( "effects/fx_unit_common.plist", "effects/fx_unit_common.png" );
}

void ResourceManager::loadUnitData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "unit.json" );
    rapidjson::Document unit_config_json;
    unit_config_json.Parse<0>( data_string.c_str() );
    _unit_config = CocosUtils::jsonObjectToValueMap( unit_config_json );
}

void ResourceManager::loadUnitLevelupCostData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "level_up_cost_conf.json" );
    rapidjson::Document unit_levelup_cost_config_json;
    unit_levelup_cost_config_json.Parse<0>( data_string.c_str() );
    _unit_levelup_cost_config = CocosUtils::jsonObjectToValueMap( unit_levelup_cost_config_json );
}

void ResourceManager::loadBulletData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "bullet.json" );
    rapidjson::Document bullet_config_json;
    bullet_config_json.Parse<0>( data_string.c_str() );
    _bullet_config = CocosUtils::jsonObjectToValueMap( bullet_config_json );
}

void ResourceManager::loadCenterData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "vision_center.json" );
    rapidjson::Document vision_config_json;
    vision_config_json.Parse<0>( data_string.c_str() );
    _vision_config = CocosUtils::jsonObjectToValueMap( vision_config_json );
}

void ResourceManager::loadBuildingData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "building.json" );
    rapidjson::Document building_config_json;
    building_config_json.Parse<0>( data_string.c_str() );
    _building_config = CocosUtils::jsonObjectToValueMap( building_config_json );
}

void ResourceManager::loadBattleUIData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "item.json" );
    rapidjson::Document battle_ui_config_json;
    battle_ui_config_json.Parse<0>( data_string.c_str() );
    _battle_ui_config = CocosUtils::jsonObjectToValueMap( battle_ui_config_json );
}

void ResourceManager::loadSkillData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "skill_conf.json" );
    rapidjson::Document skill_config_json;
    skill_config_json.Parse<0>( data_string.c_str() );
    _skill_config = CocosUtils::jsonObjectToValueMap( skill_config_json );
}

void ResourceManager::loadTeamLevelExpData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "team_level_exp.json" );
    rapidjson::Document level_exp_config_json;
    level_exp_config_json.Parse<0>( data_string.c_str() );
    _team_level_exp_config = CocosUtils::jsonArrayToValueVector( level_exp_config_json["team_level_exp"] );
}

void ResourceManager::loadTeamSkillExpData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "team_skill_exp_conf.json" );
    rapidjson::Document team_skill_exp_config_json;
    team_skill_exp_config_json.Parse<0>( data_string.c_str() );
    _team_skill_exp_config = CocosUtils::jsonObjectToValueMap( team_skill_exp_config_json );
}

void ResourceManager::loadUnitEffects() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "effects/fx_unit_common.plist", "effects/fx_unit_common.png" );
    Animation* animation = this->createAnimation( UNIT_BLEED, 5, 0.04, 1 );
    AnimationCache::getInstance()->addAnimation( animation, UNIT_BLEED );
}

void ResourceManager::loadLevelData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "level.json" );
    rapidjson::Document level_config_json;
    level_config_json.Parse<0>( data_string.c_str() );
    _level_config = CocosUtils::jsonObjectToValueMap( level_config_json );
}

void ResourceManager::loadBulletArmature( const std::string& name, const std::string& type ) {
    if( type == "spine" ) {
        std::string path = Utils::stringFormat( "effects/bullets/%s", name.c_str() );
        ArmatureManager::getInstance()->loadArmatureData( path );
    }
    else if( type == "cocos" ) {
        std::string path = Utils::stringFormat( "effects/bullets/%s/skeleton.ExportJson", name.c_str() );
        cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo( path );
    }
}

void ResourceManager::purgeBulletArmature( const std::string& name, const std::string& type ) {
    if( type == "spine" ) {
        std::string path = Utils::stringFormat( "effects/bullets/%s", name.c_str() );
        ArmatureManager::getInstance()->unloadArmatureData( path );
    }
    else if( type == "cocos" ) {
        std::string path = Utils::stringFormat( "effects/bullets/%s/skeleton.ExportJson", name.c_str() );
        cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo( path );
    }
}

const cocos2d::ValueMap& ResourceManager::getUnitData( const std::string& name ) {
    return _unit_config.at( name ).asValueMap();
}

const cocos2d::ValueMap& ResourceManager::getBulletData( const std::string& name ) {
    return _bullet_config.at( name ).asValueMap();
}

const ValueMap& ResourceManager::getSkillData( const std::string& name ) {
    return _skill_config.at( name ).asValueMap();
}

void ResourceManager::loadUnitArmatures( const cocos2d::ValueVector& armature_names ) {
    for( auto itr = armature_names.begin(); itr != armature_names.end(); ++itr ) {
        std::string unit_name = itr->asString();
        const ValueMap& unit_config = this->getUnitData( unit_name );
        bool is_double_face = unit_config.at( "double_face" ).asBool();
        if( is_double_face ) {
            std::string path = Utils::stringFormat( "characters/%s", unit_name.c_str() );
            ArmatureManager::getInstance()->loadArmatureData( path );
        }
        else {
            std::string front_path = Utils::stringFormat( "characters/%s/%s_f", unit_name.c_str() );
            ArmatureManager::getInstance()->loadArmatureData( front_path );
            std::string back_path = Utils::stringFormat( "chacaters/%s/%s_b", unit_name.c_str() );
            ArmatureManager::getInstance()->loadArmatureData( back_path );
        }
        bool is_melee = unit_config.at( "is_melee" ).asBool();
        if( !is_melee ) {
            auto itr = unit_config.find( "bullet_name" );
            if( itr != unit_config.end() ) {
                std::string bullet_name = itr->second.asString();
                if( bullet_name != "" ) {
                    const cocos2d::ValueMap& single_bullet_config = this->getBulletData( bullet_name );
                    itr = single_bullet_config.find( "body_type" );
                    if( itr != single_bullet_config.end() ) {
                        std::string resource_name = single_bullet_config.at( "name" ).asString() + "_body";
                        std::string type = single_bullet_config.at( "body_type" ).asString();
                        this->loadBulletArmature( resource_name, type );
                    }
                    itr = single_bullet_config.find( "start_type" );
                    if( itr != single_bullet_config.end() ) {
                        std::string resource_name = single_bullet_config.at( "name" ).asString() + "_start";
                        std::string type = itr->second.asString();
                        this->loadBulletArmature( resource_name, type );
                    }
                    itr = single_bullet_config.find( "hit_type" );
                    if( itr != single_bullet_config.end() ) {
                        std::string resource_name = single_bullet_config.at( "hit_name" ).asString() + "_hit";
                        std::string type = single_bullet_config.at( "hit_type" ).asString();
                        this->loadBulletArmature( resource_name, type );
                    }
                }
            }
        }
    }
}

void ResourceManager::purgeUnitArmatures( const cocos2d::ValueVector& armature_names ) {
    for( auto itr = armature_names.begin(); itr != armature_names.end(); ++itr ) {
        std::string unit_name = itr->asString();
        const ValueMap& unit_config = this->getUnitData( unit_name );
        bool is_double_face = unit_config.at( "double_face" ).asBool();
        if( is_double_face ) {
            std::string path = Utils::stringFormat( "characters/%s", unit_name.c_str() );
            ArmatureManager::getInstance()->unloadArmatureData( path );
        }
        else {
            std::string front_path = Utils::stringFormat( "characters/%s/%s_f", unit_name.c_str() );
            ArmatureManager::getInstance()->unloadArmatureData( front_path );
            std::string back_path = Utils::stringFormat( "chacaters/%s/%s_b", unit_name.c_str() );
            ArmatureManager::getInstance()->unloadArmatureData( back_path );
        }
        bool is_melee = unit_config.at( "is_melee" ).asBool();
        if( !is_melee ) {
            auto itr = unit_config.find( "bullet_name" );
            if( itr != unit_config.end() ) {
                std::string bullet_name = itr->second.asString();
                if( bullet_name != "" ) {
                    const ValueMap& single_bullet_config = this->getBulletData( bullet_name );
                    itr = single_bullet_config.find( "body_type" );
                    if( itr != single_bullet_config.end() ) {
                        std::string resource_name = single_bullet_config.at( "name" ).asString() + "_body";
                        std::string type = itr->second.asString();
                        this->purgeBulletArmature( resource_name, type );
                    }
                    itr = single_bullet_config.find( "start_type" );
                    if( itr != single_bullet_config.end() ) {
                        std::string resource_name = single_bullet_config.at( "name" ).asString() + "_start";
                        std::string type = itr->second.asString();
                        this->purgeBulletArmature( resource_name, type );
                    }
                    itr = single_bullet_config.find( "hit_type" );
                    if( itr != single_bullet_config.end() ) {
                        std::string resource_name = single_bullet_config.at( "hit_name" ).asString() + "_hit";
                        std::string type = itr->second.asString();
                        this->purgeBulletArmature( resource_name, type );
                    }
                }
            }
        }
    }
}

void ResourceManager::loadBuildingArmature( const std::string& name ) {
    
}

void ResourceManager::purgeBuildingArmature( const std::string& name ) {
    
}

cocos2d::Animation* ResourceManager::createAnimation( const std::string& name, int count, float delay, int loops ) {
    cocos2d::Vector<cocos2d::SpriteFrame*> frame_array;
    SpriteFrameCache* frame_cache = SpriteFrameCache::getInstance();
    for( int i = 1; i <= count; i++ ) {
        std::string frame_name = Utils::stringFormat( "%s_%02d.png", name.c_str(), i );
        frame_array.pushBack( frame_cache->getSpriteFrameByName( frame_name ) );
    }
    return Animation::createWithSpriteFrames( frame_array, delay, loops );
}

cocos2d::Sprite* ResourceManager::getAnimationSprite( const std::string& name ) {
    return nullptr;
}

std::string ResourceManager::getPathForResource( const std::string& name, eResourceType type ) {
    if( type == eResourceType::Character_Double_Face ) {
        return Utils::stringFormat( "characters/%s", name.c_str() );
    }
    else if( type == eResourceType::Character_Front ) {
        return Utils::stringFormat( "characters/%s/%s_f", name.c_str(), name.c_str() );
    }
    else if( type == eResourceType::Character_Back ) {
        return Utils::stringFormat( "characters/%s/%s_b", name.c_str(), name.c_str() );
    }
    else {
        return std::string( "" );
    }
}