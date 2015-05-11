//
//  UIBattleMenuLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/5/15.
//
//

#include "UIBattleMenuLayer.h"
#include "BattleLayer.h"
#include "../manager/SceneManager.h"
#include "../AI/Terrain.h"

using namespace cocos2d;

#define WIN_PANEL_FILE "ui/page/ui_win_page.csb"
#define PAUSE_PANEL_FILE "ui/page/ui_pause_page.csb"

UIBattleMenuLayer::UIBattleMenuLayer() {
    
}

UIBattleMenuLayer::~UIBattleMenuLayer() {
    
}

UIBattleMenuLayer* UIBattleMenuLayer::create( BattleLayer* battle_layer ) {
    UIBattleMenuLayer* ret = new UIBattleMenuLayer();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleMenuLayer::init( BattleLayer* battle_layer ) {
    if( !Layer::init() ) {
        return false;
    }
    
    this->setAnchorPoint( Point::ZERO );
    this->setPosition( Point::ZERO );
    this->ignoreAnchorPointForPosition( false );
    
    Size size = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    _battle_layer = battle_layer;
    
    std::string win_panel_file = FileUtils::getInstance()->fullPathForFilename( WIN_PANEL_FILE );
    _win_panel = cocos2d::CSLoader::getInstance()->createNode( win_panel_file.c_str() );
    this->addChild( _win_panel );
    _win_panel->setVisible( false );
    
    ui::Button* btn_confirm = dynamic_cast<ui::Button*>( _win_panel->getChildByName( "btn_ok" ) );
    btn_confirm->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onConfirmTouched, this ) );
    
    std::string pause_panel_file = FileUtils::getInstance()->fullPathForFilename( PAUSE_PANEL_FILE );
    _pause_panel = cocos2d::CSLoader::getInstance()->createNode( pause_panel_file.c_str() );
    this->addChild( _pause_panel );
    _pause_panel->setVisible( false );
    
    ui::Button* btn_home = dynamic_cast<ui::Button*>( _pause_panel->getChildByName( "btn_home" ) );
    btn_home->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onHomeTouched, this ) );
    
    ui::Button* btn_restart = dynamic_cast<ui::Button*>( _pause_panel->getChildByName( "btn_restart" ) );
    btn_restart->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onRestartTouched, this ) );
    
    ui::Button* btn_continue = dynamic_cast<ui::Button*>( _pause_panel->getChildByName( "btn_continue" ) );
    btn_continue->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onContinueTouched, this ) );
    
    _btn_pause = ui::Button::create( "pause_normal.png", "pause_push.png" );
    _btn_pause->setPosition( Point( origin.x + 50.0f, origin.y + size.height - 50.0f ) );
    _btn_pause->setAnchorPoint( Point( 0, 1.0f ) );
    _btn_pause->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onPauseTouched, this ) );
    this->addChild( _btn_pause );
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( UIBattleMenuLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( UIBattleMenuLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( UIBattleMenuLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( UIBattleMenuLayer::onTouchEnded, this );
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    return true;
}

void UIBattleMenuLayer::onPauseTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        this->showPausePanel();
    }
}

void UIBattleMenuLayer::onHomeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        SceneManager::getInstance()->transitToScene( eSceneName::SceneLevelChoose );
    }
}

void UIBattleMenuLayer::onRestartTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        
    }
}

void UIBattleMenuLayer::onContinueTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        this->hideMenu();
    }
}

void UIBattleMenuLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        SceneManager::getInstance()->transitToScene( eSceneName::SceneLevelChoose );
    }
}

void UIBattleMenuLayer::showResultPanel( bool win ) {
    _btn_pause->setVisible( false );
    _win_panel->setVisible( true );
    _pause_panel->setVisible( false );
    if( win ) {
        
    }
    else {
        
    }
}

void UIBattleMenuLayer::showPausePanel() {
    _btn_pause->setVisible( false );
    _win_panel->setVisible( false );
    _pause_panel->setVisible( true );
}

void UIBattleMenuLayer::hideMenu() {
    _btn_pause->setVisible( true );
    _win_panel->setVisible( false );
    _pause_panel->setVisible( false );
}

bool UIBattleMenuLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _btn_pause->isVisible() ) {
        return false;
    }
    return true;
}

void UIBattleMenuLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleMenuLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleMenuLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}