//
//  ResourceManager.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__ResourceManager__
#define __Boids__ResourceManager__

#include "../MapData.h"
#include "external/json/document.h"

enum eResourceType {
    Character_Double_Face = 1,
    Character_Front = 2,
    Character_Back = 3
};

class ResourceManager {
private:
    rapidjson::Document _unit_config;
    rapidjson::Document _vision_config;
    rapidjson::Document _building_config;
    rapidjson::Document _bullet_config;
    rapidjson::Document _battle_ui_config;
    rapidjson::Document _level_config;
    
private:
    ResourceManager();
    static ResourceManager* _instance;
    
public:
    ~ResourceManager();
    
    static ResourceManager* getInstance();
    static void destroy();
    
    const rapidjson::Document& getUnitConfig() { return _unit_config; }
    const rapidjson::Document& getVisionConfig() { return _vision_config; }
    const rapidjson::Document& getBuildingConfig() { return _building_config; }
    const rapidjson::Document& getBulletConfig() { return _bullet_config; }
    const rapidjson::Document& getBattleUIConfig() { return _battle_ui_config; }
    const rapidjson::Document& getLevelConfig() { return _level_config; }
    
    void loadMap( MapData* map_data );
    void purgeMap( MapData* map_data );
    
    void loadAllData();
    
    void loadDefaultData();
    
    void loadUnitData();
    void loadBulletData();
    void loadCenterData();
    void loadBuildingData();
    void loadBattleUIData();
    void loadUnitEffects();
    void loadLevelData();
    
    void loadBulletArmature( const std::string& name, const std::string& type );
    void purgeBulletArmature( const std::string& name, const std::string& type );
    
    const rapidjson::Value& getUnitData( const std::string& name );
    const rapidjson::Value& getBulletData( const std::string& name );
    
    void loadUnitArmatures( const rapidjson::Value& armature_names );
    void purgeUnitArmatures( const rapidjson::Value& armature_names );
    
    void loadBuildingArmature( const std::string& name );
    void purgeBuildingArmature( const std::string& name );
    
    static cocos2d::Animation* createAnimation( const std::string& name, int count, float delay, int loops );
    static cocos2d::Sprite* getAnimationSprite( const std::string& name );
    
    std::string getPathForResource( const std::string& name, eResourceType type );
};

#endif /* defined(__Boids__ResourceManager__) */
