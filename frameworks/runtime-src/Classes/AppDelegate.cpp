#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "lua_module_register.h"
//#include "lua_boids_auto.hpp"
//#include "lua_boids_manual.h"
//#include "lua_cjson.h"
#include "./manager/SceneManager.h"
#include "./manager/ResourceManager.h"
#include "./data/PlayerInfo.h"
#include "./Editor/EditMode.h"

#undef CC_USE_PHYSICS

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	FileUtils::getInstance()->setPopupNotify(false); //否则总弹各种"fullPathForFilename: No file found at blablabla"

//    auto engine = LuaEngine::getInstance();
//    ScriptEngineManager::getInstance()->setScriptEngine(engine);
//    lua_State* L = engine->getLuaStack()->getLuaState();
//    lua_module_register(L);
//    lua_getglobal(L, "_G");
//    if (lua_istable(L,-1))//stack:...,_G,
//    {
//        register_all_boids(L);
//        register_all_boids_manual(L);
//    }
//    lua_pop(L, 1);
//    luaopen_cjson(L);
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect( "My Game", cocos2d::Rect( 0, 0, 1440, 800 ) );
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize( 1920.0f, 1080.0f, ResolutionPolicy::NO_BORDER );
    
    // turn on display FPS
    director->setDisplayStats(true);
    
    director->setAnimationInterval(1.0 / 30);
    
    FileUtils* file_utils = FileUtils::getInstance();
    file_utils->addSearchPath( "res" );
    file_utils->addSearchPath( "res/BoidsUI/res" );
    file_utils->addSearchPath( "res/BoidsEditor/res" );

    // If you want to use Quick-Cocos2d-X, please uncomment below code
    // register_all_quick_manual(L);

//    if (engine->executeScriptFile("src/main.lua")) {
//        return false;
//    }
    
    ResourceManager::getInstance()->loadAllData();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    EditMode::getInstance()->enterMain( CC_CALLBACK_0( AppDelegate::onEditModeExit, this ) );
    
#else
    PlayerInfo::getInstance()->loadPlayerInfo();
    SceneManager::getInstance()->transitToScene( eSceneName::LevelChoose );
#endif
    
    return true;
}

void AppDelegate::onEditModeExit() {
    PlayerInfo::getInstance()->loadPlayerInfo();
    SceneManager::getInstance()->transitToScene( eSceneName::LevelChoose );
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
