//
//  TransitionLayer.h
//  Boids
//
//  Created by chenyanjie on 5/4/15.
//
//

#ifndef __Boids__TransitionLayer__
#define __Boids__TransitionLayer__

#include "cocos2d.h"
#include "SceneBase.h"
#include "UIHomeLayer.h"
#include "BattleLayer.h"
#include "TutorialLayer.h"
#include "OpenningAnimationLayer.h"

enum eSceneName {
    SceneUnknown = 0,
    SceneOpenning = 1,
    SceneLevelChoose = 2,
    SceneBattle = 3,
    SceneBattleTutorial = 4
};

class SceneConfig : public cocos2d::Ref {
public:
    cocos2d::Vector<cocos2d::Ref*> ref_params;
    cocos2d::ValueMap value_params;
    
    SceneConfig();
    virtual ~SceneConfig();
    
    static SceneConfig* create( const cocos2d::Vector<cocos2d::Ref*>& a_ref_params, const cocos2d::ValueMap& a_value_params );
    virtual bool init( const cocos2d::Vector<cocos2d::Ref*>& a_ref_params, const cocos2d::ValueMap& a_value_params );
};

class TransitionLayer : public cocos2d::Layer {
private:
    eSceneName _next_scene_name;
    SceneConfig* _next_scene_config;
    
public:
    TransitionLayer();
    virtual ~TransitionLayer();
    
    static TransitionLayer* create( eSceneName name, SceneConfig* config = nullptr );
    virtual bool init( eSceneName name, SceneConfig* config );
    
    eSceneName getNextSceneName() { return _next_scene_name; }
    void setNextSceneName( eSceneName name ) { _next_scene_name = name; }
    
    SceneConfig* getSceneConfig() { return _next_scene_config; }
    void setSceneConfig( SceneConfig* new_config );
    
    void transitUpdate( float delta );
};

#endif /* defined(__Boids__TransitionLayer__) */
