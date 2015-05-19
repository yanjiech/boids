//
//  UIHeroDetailLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#include "UIHeroDetailLayer.h"

using namespace cocos2d;

#define HERO_DETAIL_FILE "ui/page/ui_hero_detail.csb"

UIHeroDetailLayer::UIHeroDetailLayer() {
    
}

UIHeroDetailLayer::~UIHeroDetailLayer() {
    
}

UIHeroDetailLayer* UIHeroDetailLayer::create() {
    UIHeroDetailLayer* ret = new UIHeroDetailLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroDetailLayer::init() {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_DETAIL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_back" ) );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onBackTouched, this ) );
    
    return true;
}

void UIHeroDetailLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}