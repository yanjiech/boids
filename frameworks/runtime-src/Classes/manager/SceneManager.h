//
//  SceneManager.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__SceneManager__
#define __Boids__SceneManager__

#include "cocos2d.h"
#include <vector>
#include "../util/CommonUtils.h"

enum eSceneName {
    SceneUnknown = 0,
    SceneLevelChoose = 1,
    SceneBattle
};

class SceneConfig {
public:
    std::vector<cocos2d::Ref*> ref_params;
    cocos2d::ValueMap value_params;
};

class SceneManager {
private:
    eSceneName _current_scene_name;
    
private:
    SceneManager();
    
    static SceneManager* _instance;
    
public:
    ~SceneManager();
    
    static SceneManager* getInstance();
    static void destroy();
    
    bool transitToScene( eSceneName name, SceneConfig* config = nullptr );
};

#endif /* defined(__Boids__SceneManager__) */
