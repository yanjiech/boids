//
//  OpenningAnimationLayer.cpp
//  Boids
//
//  Created by chenyanjie on 6/10/15.
//
//

#include "OpenningAnimationLayer.h"
#include "../Utils.h"
#include "../manager/SceneManager.h"

#define OPENNING_PANEL_FILE "ui_begin.csb"

#define TOTAL_SHOT 7

using namespace cocos2d;
using namespace cocostudio::timeline;

OpenningAnimationLayer::OpenningAnimationLayer() {
    
}

OpenningAnimationLayer::~OpenningAnimationLayer() {
    ActionTimelineCache::getInstance()->removeAction( OPENNING_PANEL_FILE );
}

OpenningAnimationLayer* OpenningAnimationLayer::create() {
    OpenningAnimationLayer* ret = new OpenningAnimationLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool OpenningAnimationLayer::init() {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( OPENNING_PANEL_FILE );
    _root_node = CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    _panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( root_csb_file );
    
    _btn_next = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_next" ) );
    _btn_next->addTouchEventListener( CC_CALLBACK_2( OpenningAnimationLayer::onNextTouched, this ) );
    _btn_next->setVisible( false );
    
    _sp_next = dynamic_cast<Sprite*>( _root_node->getChildByName( "sp_next_arrow" ) );
    _sp_next->setVisible( false );
    
    ui::Button* btn_skip = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_skip" ) );
    btn_skip->addTouchEventListener( CC_CALLBACK_2( OpenningAnimationLayer::onSkipTouched, this ) );
    
    Node* pn_last = _root_node->getChildByName( "movie8_panel" );
    
    _btn_enter = dynamic_cast<ui::Button*>( pn_last->getChildByName( "btn_enter" ) );
    _btn_enter->addTouchEventListener( CC_CALLBACK_2( OpenningAnimationLayer::onEnterTouched, this ) );
    _btn_enter->setEnabled( false );

    ui::Layout* solder_layout = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "movie2_panel" ) );
    //princess
    Node* nd_princess = solder_layout->getChildByName( "nd_princess" );
    spine::SkeletonAnimation* princess_skeleton = ArmatureManager::getInstance()->createArmature( "characters/princess" );
    nd_princess->addChild( princess_skeleton );
    princess_skeleton->setAnimation( 0, "animation", true );
    
    //soldier
    for( int i = 1; i <= 3; i++ ) {
        std::string nd_name = Utils::stringFormat( "nd_soldier_%d", i );
        Node* nd_soldier = solder_layout->getChildByName( nd_name );
        spine::SkeletonAnimation* soldier_skeleton = ArmatureManager::getInstance()->createArmature( "characters/soldier" );
        nd_soldier->addChild( soldier_skeleton );
        soldier_skeleton->setAnimation( 0, "animation", true );
    }
                                                                                                 
    Node* nd_movie8 = _root_node->getChildByName( "movie8_panel" )->getChildByName( "Panel_1" );
    for( int i = 1; i <= 3; i++ ) {
        std::string nd_name = Utils::stringFormat( "nd_soldier_%d", i );
        Node* nd_soldier = nd_movie8->getChildByName( nd_name );
        spine::SkeletonAnimation* soldier_skeleton = ArmatureManager::getInstance()->createArmature( "characters/soldier" );
        nd_soldier->addChild( soldier_skeleton );
        soldier_skeleton->setAnimation( 0, "animation", true );
    }
    
    _current_shot = 1;
    _total_shot = TOTAL_SHOT;
    
    return true;
}

bool OpenningAnimationLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    return true;
}

void OpenningAnimationLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void OpenningAnimationLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void OpenningAnimationLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void OpenningAnimationLayer::onSkipTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        this->end();
    }
}

void OpenningAnimationLayer::onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        if( ++_current_shot <= _total_shot ) {
            _panel_action->play( Utils::stringFormat( "page%d", _current_shot ), false );
            _btn_next->setVisible( false );
            _sp_next->setVisible( false );
        }
    }
}

void OpenningAnimationLayer::onEnterTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        MapData* map_data = MapData::create( "maps/tutorial/map.tmx", "maps/tutorial/easy/meta.json" );
        Vector<cocos2d::Ref*> ref_params;
        ref_params.pushBack( map_data );
        ValueMap value_params;
        value_params["is_pvp"] = Value( false );
        value_params["level_id"] = Value( "10000" );
        SceneConfig* config = SceneConfig::create( ref_params, value_params );
        SceneManager::getInstance()->transitToScene( eSceneName::SceneBattle, config );
    }
}

void OpenningAnimationLayer::start( float delta ) {
    this->runAction( _panel_action );
    _panel_action->play( "page1", false );
    _panel_action->setLastFrameCallFunc( CC_CALLBACK_0( OpenningAnimationLayer::onAnimationComplete, this ) );
}

void OpenningAnimationLayer::end() {
    _panel_action->play( Utils::stringFormat( "page%d", _total_shot ), false );
    _panel_action->gotoFrameAndPause( _panel_action->getEndFrame() );
    _btn_enter->setEnabled( true );
}

void OpenningAnimationLayer::onEnterTransitionDidFinish() {
    this->scheduleOnce( CC_CALLBACK_1( OpenningAnimationLayer::start, this ), 0, "openning_start" );
}

void OpenningAnimationLayer::onAnimationComplete() {
    if( _current_shot < _total_shot ) {
        _btn_next->setVisible( true );
        _sp_next->setVisible( true );
    }
    else {
        _btn_enter->setEnabled( true );
    }
}