//
//  UIHeroManageLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#include "UIHeroManageLayer.h"
#include "UIHeroDetailLayer.h"

#define HERO_MANAGE_FILE "ui/page/ui_hero_manage.csb"

using namespace cocos2d;

UIHeroManageLayer::UIHeroManageLayer() {
    
}

UIHeroManageLayer::~UIHeroManageLayer() {
    
}

UIHeroManageLayer* UIHeroManageLayer::create() {
    UIHeroManageLayer* ret = new UIHeroManageLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroManageLayer::init() {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_MANAGE_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_confirm = dynamic_cast<ui::Button*>( _root_node->getChildByName( "finishButton" ) );
    btn_confirm->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onConfirmTouched, this ) );
    
    ui::Button* btn_detail = dynamic_cast<ui::Button*>( _root_node->getChildByName( "equipButton" ) );
    btn_detail->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onDetailTouched, this ) );
    
    return true;
}

void UIHeroManageLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        this->removeFromParentAndCleanup( true );
    }
}

void UIHeroManageLayer::onDetailTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        UIHeroDetailLayer* hero_detail = UIHeroDetailLayer::create();
        this->addChild( hero_detail, 2 );
    }
}