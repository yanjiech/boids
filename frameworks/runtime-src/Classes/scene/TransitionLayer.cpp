//
//  TransitionLayer.cpp
//  Boids
//
//  Created by chenyanjie on 5/4/15.
//
//

#include "TransitionLayer.h"
#include "../manager/ResourceManager.h"
#include "../ArmatureManager.h"
#include "../AI/Terrain.h"

using namespace cocos2d;

SceneConfig::SceneConfig() {
    
}

SceneConfig::~SceneConfig() {
    
}

SceneConfig* SceneConfig::create( const cocos2d::Vector<cocos2d::Ref*>& a_ref_params, const cocos2d::ValueMap& a_value_params ) {
    SceneConfig* ret = new SceneConfig();
    if( ret && ret->init( a_ref_params, a_value_params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SceneConfig::init( const cocos2d::Vector<cocos2d::Ref*>& a_ref_params, const cocos2d::ValueMap& a_value_params ) {
    ref_params = a_ref_params;
    value_params = a_value_params;
    
    return true;
}

//transition layer
TransitionLayer::TransitionLayer() :
_next_scene_config( nullptr )
{
    
}
    
TransitionLayer::~TransitionLayer() {
    CC_SAFE_RELEASE( _next_scene_config );
}
    
TransitionLayer* TransitionLayer::create( eSceneName name, SceneConfig* config ) {
    TransitionLayer* ret = new TransitionLayer();
    if( ret && ret->init( name, config ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TransitionLayer::init( eSceneName name, SceneConfig* config ) {
    if( !Layer::init() ) {
        return false;
    }
    
    _next_scene_name = name;
    this->setSceneConfig( config );
    
    this->scheduleOnce( CC_CALLBACK_1( TransitionLayer::transitUpdate, this ), 0, "transit_update" );
    
    return true;
}
    
void TransitionLayer::setSceneConfig( SceneConfig* new_config ) {
    CC_SAFE_RELEASE( _next_scene_config );
    _next_scene_config = new_config;
    CC_SAFE_RETAIN( _next_scene_config );
}

void TransitionLayer::transitUpdate( float delta ) {
    ResourceManager::getInstance()->unloadDefaultData();
    Terrain::getInstance()->release();
    ArmatureManager::getInstance()->clearArmatureData();
    SceneBase* next_scene = SceneBase::create();
    cocos2d::Layer* layer = nullptr;
    if( _next_scene_name == eSceneName::SceneLevelChoose ) {
        ResourceManager::getInstance()->loadUIResource();
        layer = UIHomeLayer::create();
        next_scene->addChild( layer, 10000 );
    }
    else if( _next_scene_name == eSceneName::SceneBattle ) {
        ResourceManager::getInstance()->loadBattleResource();
        bool is_pvp = _next_scene_config->value_params["is_pvp"].asBool();
        std::string level_id = _next_scene_config->value_params["level_id"].asString();
        MapData* map_data = dynamic_cast<MapData*>( _next_scene_config->ref_params.at( 0 ) );
        layer = BattleLayer::create( map_data, level_id, is_pvp );
        next_scene->addChild( layer, 10001 );
    }
    cocos2d::Director::getInstance()->replaceScene( next_scene );
}