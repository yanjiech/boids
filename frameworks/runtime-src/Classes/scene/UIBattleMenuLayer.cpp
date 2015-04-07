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
    
    Label* lb_result = Label::createWithSystemFont( result, "Arial", 128.0f );
    lb_result->setColor( Color3B::YELLOW );
    lb_result->setPosition( Point( origin.x + size.width / 2, origin.y + size.height - 200.0f ) );
    this->addChild( lb_result );
    
    Label* lb_confirm = Label::createWithSystemFont( "O K", "Arial", 128.0f );
    lb_confirm->setColor( Color3B::GREEN );
    MenuItemLabel* btn_confirm = MenuItemLabel::create( lb_confirm, CC_CALLBACK_1( UIBattleMenuLayer::onConfirmTouched, this ) );
    Menu* menu = Menu::create( btn_confirm, nullptr );
    menu->setPosition( Point( origin.x + size.width / 2, origin.y + size.height / 3 ) );
    this->addChild( menu );
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( UIBattleMenuLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( UIBattleMenuLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( UIBattleMenuLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( UIBattleMenuLayer::onTouchEnded, this );
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    return true;
}

void UIBattleMenuLayer::onConfirmTouched( cocos2d::Ref* sender ) {
    Terrain::getInstance()->release();
    SceneManager::getInstance()->transitToScene( eSceneName::SceneLevelChoose );
}

bool UIBattleMenuLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    return true;
}

void UIBattleMenuLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleMenuLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleMenuLayer::onTouchEned( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

