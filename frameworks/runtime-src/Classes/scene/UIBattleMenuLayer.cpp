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

#define WIN_PANEL_FILE ""
#define PAUSE_PANEL_FILE ""

UIBattleMenuLayer::UIBattleMenuLayer() {
    
}

UIBattleMenuLayer::~UIBattleMenuLayer() {
    
}

UIBattleMenuLayer* UIBattleMenuLayer::create( BattleLayer* battle_layer, const std::string& result ) {
    UIBattleMenuLayer* ret = new UIBattleMenuLayer();
    if( ret && ret->init( battle_layer, result ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleMenuLayer::init( BattleLayer* battle_layer, const std::string& result ) {
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
    
    std::string pause_panel_file = FileUtils::getInstance()->fullPathForFilename( PAUSE_PANEL_FILE );
    _pause_panel = cocos2d::CSLoader::getInstance()->createNode( pause_panel_file.c_str() );
    this->addChild( _pause_panel );
    
    return true;
}

void UIBattleMenuLayer::onHomeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        
    }
}

void UIBattleMenuLayer::onRestartTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        
    }
}

void UIBattleMenuLayer::onContinueTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        
    }
}

void UIBattleMenuLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        SceneManager::getInstance()->transitToScene( eSceneName::SceneLevelChoose );
    }
}

void UIBattleMenuLayer::showResultPanel() {
    _btn_pause->setVisible( false );
    _win_panel->setVisible( true );
    _pause_panel->setVisible( false );
}

void UIBattleMenuLayer::showPausePanel() {
    _btn_pause->setVisible( false );
    _win_panel->setVisible( false );
    _pause_panel->setVisible( true );
}

void UIBattleMenuLayer::hideMenu() {
    _btn_pause->setVisible( true );
}
