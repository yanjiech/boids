//
//  SceneManager.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "SceneManager.h"
#include "SceneBase.h"
#include "UILevelChooseLayer.h"
#include "BattleLayer.h"

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
    if( name == eSceneName::LevelChoose ) {
        layer = UILevelChooseLayer::create();
        next_scene->addChild( layer, 10000 );
    }
    else if( name == eSceneName::Battle ) {
        bool is_pvp = config->value_params["is_pvp"].asBool();
        MapData* map_data = dynamic_cast<MapData*>( config->ref_params[0] );
        layer = BattleLayer::create( map_data, is_pvp );
        next_scene->addChild( layer, 10001 );
    }
    cocos2d::Director* director = cocos2d::Director::getInstance();
    if( director->getRunningScene() == nullptr ) {
        director->runWithScene( next_scene );
    }
    else {
        director->replaceScene( next_scene );
    }
    return true;
}