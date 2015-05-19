//
//  UIHeroSkillLayer.cpp
//  Boids
//
//  Created by chenyanjie on 5/19/15.
//
//

#include "UIHeroSkillLayer.h"

using namespace cocos2d;

UIHeroSkillLayer::UIHeroSkillLayer() {
    
}

UIHeroSkillLayer::~UIHeroSkillLayer() {
    
}

UIHeroSkillLayer* UIHeroSkillLayer::create() {
    UIHeroSkillLayer* ret = new UIHeroSkillLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroSkillLayer::init() {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    return true;
}

void UIHeroSkillLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}