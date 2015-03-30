//
//  SceneManager.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "SceneManager.h"
#include "../scene/SceneBase.h"
#include "../scene/UILevelChooseLayer.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager() {
    
}

SceneManager::~SceneManager() {
    
}

SceneManager* SceneManager::getInstance() {
    if( _instance == nullptr ) {
        _instance = new SceneManager();
    }
    return _instance;
}

void SceneManager::destroy() {
    if( _instance ) {
        delete _instance;
        _instance = nullptr;
    }
}

bool SceneManager::transitToScene( eSceneName name, SceneConfig* config  ) {
    SceneBase* next_scene = SceneBase::create();
    cocos2d::Layer* layer = nullptr;
    if( name == eSceneName::SceneLevelChoose ) {
        layer = UILevelChooseLayer::create();
    }
    else if( name == eSceneName::SceneBattle ) {
        bool is_pvp = config->value_params["is_pvp"].asBool();
        MapData* map_data = dynamic_cast<MapData*>( config->ref_params[0] );
        layer = BattleLayer::create( map_data, is_pvp );
    }
    next_scene->addChild( layer );
    cocos2d::Director* director = cocos2d::Director::getInstance();
    if( director->getRunningScene() == nullptr ) {
        director->runWithScene( next_scene );
    }
    else {
        director->replaceScene( next_scene );
    }
    return true;
}