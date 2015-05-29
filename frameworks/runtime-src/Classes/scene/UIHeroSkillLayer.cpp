//
//  UIHeroSkillLayer.cpp
//  Boids
//
//  Created by chenyanjie on 5/19/15.
//
//

#include "UIHeroSkillLayer.h"
#include "../manager/resourceManager.h"

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
    
    _lb_level_1 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_skill_level_1" ) );
    _lb_level_2 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_skill_level_2" ) );
    
    _lb_upgrade_cost_1 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_upgrade_cost_1" ) );
    _lb_upgrade_cost_2 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_upgrade_cost_2" ) );
    
    
    _btn_upgrade_1 = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_upgrade_skill_1" ) );
    _btn_upgrade_1->setTag( 1 );
    _btn_upgrade_1->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onUpgradeTouched, this ) );
    
    _btn_upgrade_2 = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_upgrade_skill_2" ) );
    _btn_upgrade_2->setTag( 2 );
    _btn_upgrade_2->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onUpgradeTouched, this ) );
    
    _pn_hint = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_skill_hint_1" ) );
    _pn_hint->setVisible( false );
    
    ui::Layout* pn_skill_1 = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_skill_1" ) );
    _lb_skill_desc_1 = dynamic_cast<ui::Text*>( pn_skill_1->getChildByName( "lb_skill_desc_1" ) );
    
    ui::Layout* pn_skill_2 = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_skill_2" ) );
    _lb_skill_desc_2 = dynamic_cast<ui::Text*>( pn_skill_2->getChildByName( "lb_skill_desc_2" ) );
    
    ui::Button* btn_hint = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_skill_hint_1" ) );
    btn_hint->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onHintTouched, this ) );
    
    //skill_icon
    Sprite* pn_skill_icon_1 = dynamic_cast<Sprite*>( _root_node->getChildByName( "pn_skill_icon_1" ) );
    Sprite* sp_skill_icon_1 = dynamic_cast<Sprite*>( pn_skill_icon_1->getChildByName( "skill_icon" ) );
    
    
    Sprite* pn_skill_icon_2 = dynamic_cast<Sprite*>( _root_node->getChildByName( "pn_skill_icon_2" ) );
    Sprite* sp_skill_icon_2 = dynamic_cast<Sprite*>( pn_skill_icon_2->getChildByName( "skill_icon" ) );
    
    this->loadSkillData();
    
    return true;
}

void UIHeroSkillLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}

void UIHeroSkillLayer::onUpgradeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    
}

void UIHeroSkillLayer::onHintTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    
}

void UIHeroSkillLayer::loadSkillData() {
    
}