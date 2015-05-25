//
//  UIHeroSkillLayer.cpp
//  Boids
//
//  Created by chenyanjie on 5/19/15.
//
//

#include "UIHeroSkillLayer.h"

#define HERO_SKILL_FILE "ui/page/ui_hero_skill.csb"

using namespace cocos2d;

UIHeroSkillLayer::UIHeroSkillLayer() {
    
}

UIHeroSkillLayer::~UIHeroSkillLayer() {
    
}

UIHeroSkillLayer* UIHeroSkillLayer::create( UIHeroManageHeroSlot* hero ) {
    UIHeroSkillLayer* ret = new UIHeroSkillLayer();
    if( ret && ret->init( hero ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroSkillLayer::init( UIHeroManageHeroSlot* hero ) {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    _hero = hero;
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_SKILL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_back" ) );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onBackTouched, this ) );
    
    return true;
}

void UIHeroSkillLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}