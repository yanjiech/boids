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

enum eResourceType {
    Character_Double_Face = 1,
    Character_Front = 2,
    Character_Back = 3
};

class ResourceManager {
private:
    cocos2d::ValueMap _unit_config;
    cocos2d::ValueMap _vision_config;
    cocos2d::ValueMap _building_config;
    cocos2d::ValueMap _bullet_config;
    cocos2d::ValueMap _battle_ui_config;
    cocos2d::ValueMap _level_config;
    cocos2d::ValueMap _skill_config;
    
private:
    ResourceManager();
    static ResourceManager* _instance;
    
public:
    ~ResourceManager();
    
    static ResourceManager* getInstance();
    static void destroy();
    
    const cocos2d::ValueMap& getUnitConfig() { return _unit_config; }
    const cocos2d::ValueMap& getVisionConfig() { return _vision_config; }
    const cocos2d::ValueMap& getBuildingConfig() { return _building_config; }
    const cocos2d::ValueMap& getBulletConfig() { return _bullet_config; }
    const cocos2d::ValueMap& getBattleUIConfig() { return _battle_ui_config; }
    const cocos2d::ValueMap& getLevelConfig() { return _level_config; }
    const cocos2d::ValueMap& getSkillConfig() { return _skill_config; }
    
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
    void loadSkillData();
    
    void loadBulletArmature( const std::string& name, const std::string& type );
    void purgeBulletArmature( const std::string& name, const std::string& type );
    
    const cocos2d::ValueMap& getUnitData( const std::string& name );
    const cocos2d::ValueMap& getBulletData( const std::string& name );
    const cocos2d::ValueMap& getSkillData( const std::string& name );
    
    void loadUnitArmatures( const cocos2d::ValueVector& armature_names );
    void purgeUnitArmatures( const cocos2d::ValueVector& armature_names );
    
    void loadBuildingArmature( const std::string& name );
    void purgeBuildingArmature( const std::string& name );
    
    static cocos2d::Animation* createAnimation( const std::string& name, int count, float delay, int loops );
    static cocos2d::Sprite* getAnimationSprite( const std::string& name );
    
    std::string getPathForResource( const std::string& name, eResourceType type );
};

#endif /* defined(__Boids__ResourceManager__) */
