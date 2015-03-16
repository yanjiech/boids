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
    const rapidjson::Document& map_json = map_data->getMapJson();
    
    const rapidjson::Value& unit_names = map_json["units"];
    this->loadUnitArmatures( unit_names );
    
    const rapidjson::Value& player_unit_names = PlayerInfo::getInstance()->getPlayerUnitsInfo();
    this->loadUnitArmatures( player_unit_names );
}

void ResourceManager::purgeMap( MapData* map_data ) {
    map_data->removeImagesFromCache();
    const rapidjson::Document& map_json = map_data->getMapJson();
    
    const rapidjson::Value& unit_names = map_json["units"];
    this->purgeUnitArmatures( unit_names );
    
    const rapidjson::Value& player_unit_names = PlayerInfo::getInstance()->getPlayerUnitsInfo();
    this->purgeUnitArmatures( player_unit_names );
}

void ResourceManager::loadAllData() {
    this->loadDefaultData();
    this->loadUnitData();
    this->loadBulletData();
    this->loadCenterData();
    this->loadBuildingData();
    this->loadBattleUIData();
    this->loadLevelData();
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
    _unit_config.Parse<0>( data_string.c_str() );
}

void ResourceManager::loadBulletData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "bullet.json" );
    _bullet_config.Parse<0>( data_string.c_str() );
}

void ResourceManager::loadCenterData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "vision_center.json" );
    _vision_config.Parse<0>( data_string.c_str() );
}

void ResourceManager::loadBuildingData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "building.json" );
    _building_config.Parse<0>( data_string.c_str() );
}

void ResourceManager::loadBattleUIData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "item.json" );
    _battle_ui_config.Parse<0>( data_string.c_str() );
}

void ResourceManager::loadUnitEffects() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "effects/fx_unit_common.plist", "effects/fx_unit_common.png" );
    Animation* animation = this->createAnimation( UNIT_BLEED, 5, 0.04, 1 );
    AnimationCache::getInstance()->addAnimation( animation, UNIT_BLEED );
}

void ResourceManager::loadLevelData() {
    std::string data_string = FileUtils::getInstance()->getStringFromFile( "level.json" );
    _level_config.Parse<0>( data_string.c_str() );
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

const rapidjson::Value& ResourceManager::getUnitData( const std::string& name ) {
    return _unit_config[name.c_str()];
}

const rapidjson::Value& ResourceManager::getBulletData( const std::string& name ) {
    return _bullet_config[name.c_str()];
}

void ResourceManager::loadUnitArmatures( const rapidjson::Value& armature_names ) {
    for( rapidjson::Value::ConstMemberIterator itr = armature_names.MemberonBegin(); itr != armature_names.MemberonEnd(); ++itr ) {
        std::string unit_name = std::string( itr->value.GetString(), itr->value.GetStringLength() );
        const rapidjson::Value& unit_config = this->getUnitData( unit_name );
        bool is_double_face = unit_config["double_face"].GetBool();
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
        bool is_melee = unit_config["is_melee"].GetBool();
        if( !is_melee ) {
            if( unit_config.HasMember( "bullet_name" ) ) {
                std::string bullet_name = std::string( unit_config["bullet_name"].GetString(), unit_config["bullet_name"].GetStringLength() );
                if( bullet_name != "" ) {
                    const rapidjson::Value& single_bullet_config = this->getBulletData( bullet_name );
                    if( single_bullet_config.HasMember( "body_type" ) ) {
                        std::string resource_name = Utils::stringFormat( "%s_body", single_bullet_config["name"].GetString() );
                        std::string type = std::string( single_bullet_config["body_type"].GetString() );
                        this->loadBulletArmature( resource_name, type );
                    }
                    if( single_bullet_config.HasMember( "start_type" ) ) {
                        std::string resource_name = Utils::stringFormat( "%s_start", single_bullet_config["name"].GetString() );
                        std::string type = std::string( single_bullet_config["start_type"].GetString() );
                        this->loadBulletArmature( resource_name, type );
                    }
                    if( single_bullet_config.HasMember( "hit_type" ) ) {
                        std::string resource_name = Utils::stringFormat( "%s_hit", single_bullet_config["hit_name"].GetString() );
                        std::string type = std::string( single_bullet_config["hit_type"].GetString() );
                        this->loadBulletArmature( resource_name, type );
                    }
                }
            }
        }
    }
}

void ResourceManager::purgeUnitArmatures( const rapidjson::Value& armature_names ) {
    for( rapidjson::Value::ConstMemberIterator itr = armature_names.MemberonBegin(); itr != armature_names.MemberonEnd(); ++itr ) {
        for( rapidjson::Value::ConstMemberIterator itr = armature_names.MemberonBegin(); itr != armature_names.MemberonEnd(); ++itr ) {
            std::string unit_name = std::string( itr->value.GetString(), itr->value.GetStringLength() );
            const rapidjson::Value& unit_config = this->getUnitData( unit_name );
            bool is_double_face = unit_config["double_face"].GetBool();
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
            bool is_melee = unit_config["is_melee"].GetBool();
            if( !is_melee ) {
                if( unit_config.HasMember( "bullet_name" ) ) {
                    std::string bullet_name = std::string( unit_config["bullet_name"].GetString(), unit_config["bullet_name"].GetStringLength() );
                    if( bullet_name != "" ) {
                        const rapidjson::Value& single_bullet_config = this->getBulletData( bullet_name );
                        if( single_bullet_config.HasMember( "body_type" ) ) {
                            std::string resource_name = Utils::stringFormat( "%s_body", single_bullet_config["name"].GetString() );
                            std::string type = std::string( single_bullet_config["body_type"].GetString() );
                            this->purgeBulletArmature( resource_name, type );
                        }
                        if( single_bullet_config.HasMember( "start_type" ) ) {
                            std::string resource_name = Utils::stringFormat( "%s_start", single_bullet_config["name"].GetString() );
                            std::string type = std::string( single_bullet_config["start_type"].GetString() );
                            this->purgeBulletArmature( resource_name, type );
                        }
                        if( single_bullet_config.HasMember( "hit_type" ) ) {
                            std::string resource_name = Utils::stringFormat( "%s_hit", single_bullet_config["hit_name"].GetString() );
                            std::string type = std::string( single_bullet_config["hit_type"].GetString() );
                            this->purgeBulletArmature( resource_name, type );
                        }
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